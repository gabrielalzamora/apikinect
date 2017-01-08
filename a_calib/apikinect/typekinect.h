/*
 * Copyright (c) 2016  Gabriel Alzamora.
 * Copyright (c) 2016  Raúl Alves.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef TYPEKINECT_H
#define TYPEKINECT_H

#include <cstdint>
#include <vector>

#define	RES_BARRIDO	0.50
#define	TAM_BARRIDO	360

#define RES_KINECT_3D_H 480
#define RES_KINECT_3D_W 640

#define RES_KINECT_VIDEO_H 480
#define RES_KINECT_VIDEO_W 640

#define W 640
#define H 480

#define SRVKPORT 10003
#define DEPTHPORT 10004
#define VIDEOPORT 10005
#define THREEPORT 10006
#define TWOPORT 10007
#define BARRIDOPORT 10008
#define ACCELPORT 10009

/*!
 * \struct RGBQ
 * \brief color in RGB+A (opacity) style 32bits long.
 */
typedef struct {
    uint8_t rgbRed;//!< red color component of RGB.
    uint8_t rgbGreen;//!< green color component of RGB.
    uint8_t rgbBlue;//!< blue color component of RGB.
    uint8_t rgbReserved;//!< reserved for future use.
}RGBQ;

/*!
 * \struct RGB
 * \brief color in RGB style 24bits long.
 */
typedef struct {
    uint8_t rgbRed;//!< red color component of RGB.
    uint8_t rgbGreen;//!< green color component of RGB.
    uint8_t rgbBlue;//!< blue color component of RGB.
}rgb;

/*!
 * \struct point3c
 * \brief 3D point with rgba 32bits color information.
 *
 * Represents position and color of a point
 * in 3D space with distance in milimeters (mm)
 * and color as RGBQ color.
 */
typedef struct {
    int16_t x;//!< x dimension on 3D point
    int16_t y;//!< y dimension on 3D point
    int16_t z;//!< z dimension on 3D point
    RGBQ color;//!< RGBA color 32bits
}point3c;

/*!
 * \struct point3rgb
 * \brief 3D point with rgb 24bits color information.
 *
 * Represents position and color of a point
 * in 3D space with distance in milimeters (mm)
 * and color as RGBQ color.
 */
typedef struct point3rgb{
    int16_t x;//!< x dimension on 3D point
    int16_t y;//!< y dimension on 3D point
    int16_t z;//!< z dimension on 3D point
    rgb color;//!< RGB color 24bits
}point3rgb;

/*!
 * \struct point2
 * \brief 2D point on horizontal plane.
 */
typedef struct {
    int16_t x;
    int16_t z;
}point2;

/*!
 * \struct accel
 * \brief acceleration detected components.
 */
typedef struct {
    double accel_x;
    double accel_y;
    double accel_z;
}accel;

/*!
 * \struct srvKinect
 * \brief contain all Data information (but led).
 *
 * Used as database but mainly to send information from
 * client to server, change kinect camera angle remotely
 * and update data to be sent, limits, refresh...
 */
typedef struct SrvKinect{
    double m_fAngulo;//!< angle (degrees) with horizontal plane of kinect base.
    int8_t m_iAnguloKinect;//!< sigue cuando te estanques en otra cosa
    double m_fAltura;
    double m_fYMin;
    double m_fYMax;
    double m_fZMax;
    uint32_t m_ulRefresco3D;
    uint8_t m_usModulo3D;
    uint8_t m_bEnvio3D;
    uint8_t m_bEnvio2D;
    uint8_t m_bEnvioBarrido;
    uint8_t m_bCompress3D;
    int32_t m_iBarridoEcu;
    int32_t m_iBarridoYMin;
    int32_t m_iBarridoYMax;
    uint32_t m_ulRefrescoDepth;
    uint8_t m_bEnvioDepth;
    uint8_t m_bCompressDepth;
    uint32_t m_ulRefrescoColor;
    uint8_t m_bEnvioColor;
    uint8_t m_bCompressColor;
}srvKinect;

/*!
 * \struct pBuf
 * \brief holds pointers to all kinect and derived data
 */
typedef struct {
    std::vector<uint8_t> *ptrVideoBuf;//!< container of video info from kinect
    std::vector<uint16_t> *ptrDepthBuf;//!< container of depth info from kinect
    std::vector<point3rgb> *ptrP3Buf;//!< container of points cloud <- video+depth
    std::vector<point2> *ptrP2Buf;//!< container of 2D points = (point cloud) - color - z
    std::vector<uint32_t> *ptrBarridoBuf;//!< barridoBuf contains distance to closer object on angle (360-i)/2 degrees, xOz plane (horizontal to camera)
    accel *ptrAccel;//!< acceleration components x,y,z (y ~ 9,81 if m_iAnguloKinect=0)
}pBuf;

/*!
 * \brief to easy locate variables in vectors.
 *
 * Used in timeVector.
 */
typedef enum {
    e_video,
    e_depth,
    e_3,
    e_2,
    e_barrido,
    e_accel,
    e_ir,
    e_paint_video,
    e_paint_depth,
    e_paint_3,
    e_paint_2,
    e_paint_barrido,
    e_paint_accel,
    e_paint_ir,
    e_fly_video,
    e_fly_depth,
    e_fly_3,
    e_fly_2,
    e_fly_barrido,
    e_fly_accel,
    e_fly_ir,
    e_xtra
} eOption;

#endif // TYPEKINECT_H
