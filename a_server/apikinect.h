/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef APIKINECT_H
#define APIKINECT_H

#include <mutex>
#include <vector>
#include <libfreenect.hpp>
#include "data.h"

class Apikinect : public Freenect::FreenectDevice
{

public:
    explicit Apikinect(freenect_context *_ctx, int _index);

public:
    void VideoCallback(void *_rgb, uint32_t timestamp);// Do not call directly, even in child
    void DepthCallback(void *_depth, uint32_t timestamp);// Do not call directly, even in child
    bool getRGB(std::vector<uint8_t> &buffer);
    bool getDepth(std::vector<uint16_t> &buffer);

    void getAll(std::vector<point3c> &buffer3,std::vector<uint32_t> &bufferB);//
    void getAll(pBuf *structBuffers, srvKinect *aSrvKinect);

    void get3d(std::vector<point3c> &buffer);
    void get3d(pBuf *structBuffers, srvKinect *aSrvKinect);

    void get2(std::vector<point2> &buffer);
    void get2(pBuf *structBuffers, srvKinect *aSrvKinect);

    void getBarrido(std::vector<uint32_t> &buffer);
    void getBarrido(pBuf *structBuffers, srvKinect *aSrvKinect);

    void get3dBarrido(pBuf *structBuffers, srvKinect *aSrvKinect);
    void get2dBarrido(pBuf *structBuffers, srvKinect *aSrvKinect);
    void get2and3(pBuf *structBuffers, srvKinect *aSrvKinect);

    void getAccel(accel &a);

private:
    freenect_device * myself;
    std::mutex m_rgb_mutex;//!< mutex to ensure video frame data save access
    std::mutex m_depth_mutex;//!< mutex to ensure video frame data save access
//    std::mutex m_mutex_3D;//!< mutex to protect 3D data read/write operations
//    std::mutex m_mutex_barre;//!< mutex to ensure save barre read/write operations
    std::vector<uint8_t> m_buffer_video;//!< buffer to store usb incoming video frame (see VideoCallback)
    std::vector<uint16_t> m_buffer_depth;//!< buffer to store usb incoming depth frame (see DepthCallback)
    bool m_new_rgb_frame;//!< flag true if we have new frame data in m_buffer_video, otherwise false
    bool m_new_depth_frame;//!< flag true if we have new frame data in m_buffer_depth, otherwise false

//    std::vector<point3c> m_buffer_3;//!< buffer to store three dimention plus color points
//    std::vector<point2> m_buffer_2;//!< buffer to store point cloud proyected to floor plane
//    std::vector<uint32_t> m_buffer_B;//!< buffer to hold distances seen on each azimut angle
};

#endif // APIKINECT_H
