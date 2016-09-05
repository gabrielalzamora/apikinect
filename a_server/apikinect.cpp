/*
 * Copyright (c) 2016  Gabriel Alzamora.
 * Copyright (c) 2010 individual OpenKinect contributors.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <math.h>       // sqrt(), atan()...
#include "apikinect.h"

/*!
 * \class Apikinect
 * \brief holds all kinect device properties to read & handel it.
 */

/*!
 * \brief constructor: reserve memory and set initial flags.
 * \param [out] _ctx return usb context to communicate with kinect.
 * \param [in] _index selected kinect index.
 */
Apikinect::Apikinect(freenect_context *_ctx, int _index)
    : Freenect::FreenectDevice(_ctx, _index),
      m_buffer_video(freenect_find_video_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_VIDEO_RGB).bytes),
      m_buffer_depth(freenect_find_depth_mode(FREENECT_RESOLUTION_MEDIUM, FREENECT_DEPTH_REGISTERED).bytes / 2),
      m_new_rgb_frame(false), m_new_depth_frame(false)
{
    myself = const_cast<freenect_device*>(this->getDevice());//las vueltas que da la vida :o)
    setDepthFormat(FREENECT_DEPTH_REGISTERED);
}

/*!
 * \brief called if usb video event in Freenect::m_thread.
 * \param [out] _rgb poitns buffer where to save usb incoming video frame.
 * \param timestamp internal use to synchronize video & depth frames.
 * \see libfreenect.hpp Freenect::Freenect
 */
void Apikinect::VideoCallback(void *_rgb, uint32_t timestamp)
{
    std::lock_guard<std::mutex> lock(m_rgb_mutex);//lock will be destroy when exit VideoCallback
    uint8_t* rgb = static_cast<uint8_t*>(_rgb);
    copy(rgb, rgb+getVideoBufferSize(), m_buffer_video.begin());
    m_new_rgb_frame = true;
}

/*!
 * \brief called if usb depth event in Freenect::m_thread.
 * \param [out] _depth points buffer where to save usb incoming depth frame.
 * \param timestamp internal use to synchronize video & depth frames.
 * \see libfreenect.hpp Freenect::Freenect
 */
void Apikinect::DepthCallback(void *_depth, uint32_t timestamp)
{
    std::lock_guard<std::mutex> lock(m_depth_mutex);//lock will be destroy when exit VideoCallback
    uint16_t* depth = static_cast<uint16_t*>(_depth);
    copy(depth, depth+getDepthBufferSize()/2, m_buffer_depth.begin());
    m_new_depth_frame = true;
}

/*!
 * \brief retrieve video frame.
 * \param [out] buffer points to buffer where new video frame is stored.
 * \return true if we have new video frame, false if not new.
 */
bool Apikinect::getRGB(std::vector<uint8_t> &buffer)
{
    std::lock_guard<std::mutex> lock(m_rgb_mutex);
    if (!m_new_rgb_frame)
        return false;
    buffer.swap(m_buffer_video);
    m_new_rgb_frame = false;
    return true;
}

/*!
 * \brief retrieve depth frame.
 * \param [out] buffer points to buffer where new depth frame is stored.
 * \return true if we have new depth frame, false if not new.
 */
bool Apikinect::getDepth(std::vector<uint16_t> &buffer)
{
    std::lock_guard<std::mutex> lock(m_depth_mutex);
    if (!m_new_depth_frame)
        return false;
    buffer.swap(m_buffer_depth);
    m_new_depth_frame = false;
    return true;
}

/*!
 * \brief funtion to retrieve point cloud & barrido (swept) from video & depth data.
 *
 * Return 3D point cloud (2D may be retrieved from it) and barrido (swept)
 * in camera coordinates (Oz to where camera is looking, Ox right hand and
 * Oy downward to Oz)
 * \param [out] buffer3 buffer to receive point cloud.
 * \param [out] bufferB buffer to receive Barrido (swept).
 */
void Apikinect::getAll(std::vector<point3c> &buffer3, std::vector<uint32_t> &bufferB)
{
    point3c p3;
    RGBQ color;
    buffer3.resize(0);
    for(int i=0;i<360;i++) bufferB[i]=0;
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; ++i)
    {
        // Convert from image plane coordinates to world coordinates
        if( (p3.z = m_buffer_depth[i]) != 0  ){                  // Z = d
            p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;      // X = (x - cx) * d / fx
            p3.y = (i/640 - (480-1)/2.f) * m_buffer_depth[i] / f;// Y = (y - cy) * d / fy
            color.rgbRed = m_buffer_video[3*i+0];    // R
            color.rgbGreen = m_buffer_video[3*i+1];  // G
            color.rgbBlue = m_buffer_video[3*i+2];   // B
            color.rgbReserved = 0;
            p3.color = color;
            buffer3.push_back(p3);//MainWindow::p3Buf

            int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
            uint32_t length = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm
            if( bufferB[index]==0 || bufferB[index]>length)
                bufferB[index]=length;
        }
    }
}
/*!
 * \brief funtion to retrieve 3D, 2D & Barrido from video & depth data.
 *
 * From video, depth frames and srvKinect store 3D, 2D & Barrido in
 * structBuffers buffers in world coordinates (Oz horizontal ahead,
 * Ox horizontal right hand and Oy downwards).
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::getAll(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point2 p2;//2 dimensions points cloud
    point3c p3;//3D + color points cloud
    RGBQ color;
    structBuffers->ptrP3Buf->resize(0);//reset 3D points cloud
    structBuffers->ptrP2Buf->resize(0);//reset 2D points cloud
    for(int i=0;i<360;i++) (*structBuffers->ptrBarridoBuf)[i]=0;//reset Barrido vector
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);
        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;
            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p2.z = p3.z;
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                p2.x = p3.x;
                color.rgbRed = m_buffer_video[3*i+0];    // R
                color.rgbGreen = m_buffer_video[3*i+1];  // G
                color.rgbBlue = m_buffer_video[3*i+2];   // B
                color.rgbReserved = 0;
                p3.color = color;
                structBuffers->ptrP3Buf->push_back(p3);//stored in MainWindow::p3Buf
                structBuffers->ptrP2Buf->push_back(p2);//stored in MainWindow::p2Buf

                int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
                uint32_t distance = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm

                //within Barrido limits
                if( (p3.y >= (-1)*aSrvKinect->m_iBarridoYMax) && (p3.y <= (-1)*aSrvKinect->m_iBarridoYMin) ){
                    if( (*structBuffers->ptrBarridoBuf)[index]==0 || (*structBuffers->ptrBarridoBuf)[index]>distance )
                        (*structBuffers->ptrBarridoBuf)[index]=distance;
                }
            }
        }
    }
}

/*!
 * \brief 3 dimension + color point cloud in buffer from video & depth data.
 *
 * World coordinates are from camera point of view, Oz points in
 * the same direction of the camera, Ox to the right and Oy points
 * downwards.
 * \param [out] buffer  where point cloud is stored.
 */
void Apikinect::get3d(std::vector<point3c> &buffer)
{
    point3c p3;
    RGBQ color;
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f
//time pre buffers
    for (int i = 0; i < 480*640; ++i)
    {
        // Convert from image plane coordinates to world coordinates
        if( (p3.z = m_buffer_depth[i]) != 0  ){                  // Z = d
            p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;      // X = (x - cx) * d / fx
            p3.y = (i/640 - (480-1)/2.f) * m_buffer_depth[i] / f;// Y = (y - cy) * d / fy
            color.rgbRed = m_buffer_video[3*i+0];    // R
            color.rgbGreen = m_buffer_video[3*i+1];  // G
            color.rgbBlue = m_buffer_video[3*i+2];   // B
            color.rgbReserved = 0;
            p3.color = color;
            buffer.push_back(p3);//MainWindow::p3Buf
        }
    }
}
/*!
 * \brief funtion to retrieve 3D from video & depth data.
 *
 * From video, depth frames and srvKinect store 3D in
 * structBuffers buffers in world coordinates Oz to the front
 * Ox to the right and Oy points downwards.
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::get3d(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point3c p3;//3D + color points cloud
    RGBQ color;
    structBuffers->ptrP3Buf->resize(0);//reset 3D points cloud

    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f
    //------------------------------------------------------time pre buffers
    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                color.rgbRed = m_buffer_video[3*i+0];    // R
                color.rgbGreen = m_buffer_video[3*i+1];  // G
                color.rgbBlue = m_buffer_video[3*i+2];   // B
                color.rgbReserved = 0;
                p3.color = color;
                structBuffers->ptrP3Buf->push_back(p3);//stored in MainWindow::p3Buf
            }
        }
    }
}

/*!
 * \brief 2 dimention point cloud in buffer from video & depth data.
 * \param [out] buffer where point cloud is stored.
 */
void Apikinect::get2(std::vector<point2> &buffer)
{
    point2 p2;
    buffer.resize(1);
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; ++i)
    {
        // Convert from image plane coordinates to world coordinates
        if( (p2.z = m_buffer_depth[i]) != 0  ){                  // Z = d
            p2.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;      // X = (x - cx) * d / fx
            buffer.push_back(p2);//MainWindow::p2Buf
        }
    }
}
/*!
 * \brief funtion to retrieve 2D from video & depth data.
 *
 * From video, depth frames and srvKinect store 3D in
 * structBuffers buffers in world coordinates Oz to the front
 * Ox to the right and Oy points downwards.
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::get2(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point2 p2;//2 dimensions points cloud
    point3c p3;//3D + color points cloud
    structBuffers->ptrP2Buf->resize(0);//reset 2D points cloud
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D){
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p2.z = p3.z;
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                p2.x = p3.x;

                structBuffers->ptrP2Buf->push_back(p2);//stored in MainWindow::p2Buf
            }
        }
    }
}

/*!
 * \brief distance to closer object function of angle you are looking at
 *
 * Vector index tells angle you are pointing, vector content is
 * distance to closer object. 360 size vector with index 0 containing
 * distance to closest object angle 90º to the right from where
 * camara is pointing. Range from -89.5º to 90º each 1/2 degree (180*2=360 values)
 * \param buffer where 360 distance values are stored
 */
void Apikinect::getBarrido(std::vector<uint32_t> &buffer)
{
    point3c p3;
    for(int i=0;i<360;i++) buffer[i]=0;
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; ++i)
    {
        if( (p3.z = m_buffer_depth[i]) != 0  ){                  // Z = d
            p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;      // X = (x - cx) * d / fx
            int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
            uint32_t length = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm
            if( buffer[index]==0 || buffer[index]>length)
                buffer[index]=length;
        }
    }
}
/*!
 * \brief get distance to closer object function of angle you are looking at
 *
 * Store in pBuf.ptrBarridoBuf a vector wich index tells angle you are pointing,
 * vector content is distance to closer object. 360 size vector with index 0
 * containing distance to closest object at angle 90º to the right from where
 * camara is pointing. Range from -89.5º to 90º each 1/2 degree (180*2=360 values)
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::getBarrido(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point3c p3;
    for(int i=0;i<360;i++) (*structBuffers->ptrBarridoBuf)[i]=0;
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx

                int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
                uint32_t distance = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm

                //within Barrido limits
                if( (p3.y >= (-1)*aSrvKinect->m_iBarridoYMax) && (p3.y <= (-1)*aSrvKinect->m_iBarridoYMin) ){
                    if( (*structBuffers->ptrBarridoBuf)[index]==0 || (*structBuffers->ptrBarridoBuf)[index]>distance )
                        (*structBuffers->ptrBarridoBuf)[index]=distance;
                }
            }
        }
    }
}

/*!
 * \brief obtain 3d point cloud and barrido (swept).
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 * \see getBarrido()
 */
void Apikinect::get3dBarrido(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point3c p3;//3D + color points cloud
    RGBQ color;
    structBuffers->ptrP3Buf->resize(0);//reset 3D points cloud
    for(int i=0;i<360;i++) (*structBuffers->ptrBarridoBuf)[i]=0;//reset Barrido vector
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                color.rgbRed = m_buffer_video[3*i+0];    // R
                color.rgbGreen = m_buffer_video[3*i+1];  // G
                color.rgbBlue = m_buffer_video[3*i+2];   // B
                color.rgbReserved = 0;
                p3.color = color;
                structBuffers->ptrP3Buf->push_back(p3);//stored in MainWindow::p3Buf

                int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
                uint32_t distance = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm

                //within Barrido limits
                if( (p3.y >= (-1)*aSrvKinect->m_iBarridoYMax) && (p3.y <= (-1)*aSrvKinect->m_iBarridoYMin) ){
                    if( (*structBuffers->ptrBarridoBuf)[index]==0 || (*structBuffers->ptrBarridoBuf)[index]>distance )
                        (*structBuffers->ptrBarridoBuf)[index]=distance;
                }
            }
        }
    }
}
/*!
 * \brief get 2d point cloud and Barrido (swept)
 *
 * Store them in pBuf.ptrP3Buf and pBuf.ptrBarridoBuf
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::get2dBarrido(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point2 p2;//2 dimensions points cloud
    point3c p3;//3D + color points cloud
    structBuffers->ptrP2Buf->resize(0);//reset 2D points cloud
    for(int i=0;i<360;i++) (*structBuffers->ptrBarridoBuf)[i]=0;//reset Barrido vector
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p2.z = p3.z;
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                p2.x = p3.x;
                structBuffers->ptrP2Buf->push_back(p2);//stored in MainWindow::p2Buf

                int index = 180-int(2*atan(double(p3.x)/double(p3.z))*180/M_PI);
                uint32_t distance = uint32_t(sqrt( p3.x*p3.x + p3.z*p3.z ));//distance en mm

                //within Barrido limits
                if( (p3.y >= (-1)*aSrvKinect->m_iBarridoYMax) && (p3.y <= (-1)*aSrvKinect->m_iBarridoYMin) ){
                    if( (*structBuffers->ptrBarridoBuf)[index]==0 || (*structBuffers->ptrBarridoBuf)[index]>distance )
                        (*structBuffers->ptrBarridoBuf)[index]=distance;
                }
            }
        }
    }
}
/*!
 * \brief get 3d and 2d point clouds
 *
 * Store them in pBuf.ptrP3Buf and pBuf.ptrP2Buf
 * \param [out] structBuffers points to struct holding buffers.
 * \param [in] aSrvKinect configuration data.
 */
void Apikinect::get2and3(pBuf *structBuffers, srvKinect *aSrvKinect)
{
    point2 p2;//2 dimensions points cloud
    point3c p3;//3D + color points cloud
    RGBQ color;
    structBuffers->ptrP3Buf->resize(0);//reset 3D points cloud
    structBuffers->ptrP2Buf->resize(0);//reset 2D points cloud
    float f = 595.f;//intrinsec kinect camera parameter fx=fy=f

    for (int i = 0; i < 480*640; i+=aSrvKinect->m_usModulo3D)
    {
        // Convert from image plane coordinates to world coordinates
        // Z into screen, Y downwards, X to right as OpenGL
        // Z = depth*cos(angleKinect)-y*sin(angleKinect)  if angleKinect around x axis
        p3.z = m_buffer_depth[i]*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0);

        //within z limits
        if( (p3.z != 0) && (p3.z <= aSrvKinect->m_fZMax) ){
            // Y = (y - cy) * d / fy but remember angleKinect plus camera high
            // Y = y*cos(angleKinect)+depth*sin(angleKinect)-CameraHigh if angleKinect around Ox axis
            p3.y = ((i/640-(480-1)/2.f)*m_buffer_depth[i]/f)*cos(aSrvKinect->m_iAnguloKinect*M_PI/180.0) + m_buffer_depth[i]*sin(aSrvKinect->m_iAnguloKinect*M_PI/180.0) - aSrvKinect->m_fAltura*1000;

            //within y limits
            if( (p3.y >= (-1)*aSrvKinect->m_fYMax) && (p3.y <= (-1)*aSrvKinect->m_fYMin) ){//within y limits (remember Y downwards)
                p2.z = p3.z;
                p3.x = (i%640-(640-1)/2.f)*m_buffer_depth[i]/f;// X = (x - cx) * d / fx
                p2.x = p3.x;
                color.rgbRed = m_buffer_video[3*i+0];    // R
                color.rgbGreen = m_buffer_video[3*i+1];  // G
                color.rgbBlue = m_buffer_video[3*i+2];   // B
                color.rgbReserved = 0;
                p3.color = color;
                structBuffers->ptrP3Buf->push_back(p3);//stored in MainWindow::p3Buf
                structBuffers->ptrP2Buf->push_back(p2);//stored in MainWindow::p2Buf
            }
        }
    }
}

/*!
 * \brief store acceleration  components in a, accel strut
 * \param [out] a where acceleration components is saved
 */
void Apikinect::getAccel(accel &a)
{
    freenect_update_tilt_state(myself);
    freenect_get_mks_accel(freenect_get_tilt_state(myself), &a.accel_x, &a.accel_y, &a.accel_z);
}
