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
 * \brief color in RGB style 32bits long.
 */
typedef struct RGBQ{
    uint8_t rgbRed;//!< red color component of RGB.
    uint8_t rgbGreen;//!< green color component of RGB.
    uint8_t rgbBlue;//!< blue color component of RGB.
    uint8_t rgbReserved;//!< reserved for future use.
}RGBQ;

/*!
 * \struct point3c
 * \brief 3D point with color information.
 *
 * Represents position and color of a point
 * in 3D space with distance in milimeters (mm)
 * and color as RGBQ color.
 */
typedef struct point3c{
    int16_t x;//!< sigue cuando te estanques en otra cosa
    int16_t y;
    int16_t z;
    RGBQ color;
}point3c;

/*!
 * \struct point2
 * \brief 2D point on horizontal plane.
 */
typedef struct point2{
    int16_t x;
    int16_t z;
}point2;

/*!
 * \struct accel
 * \brief acceleration detected components.
 */
typedef struct accel{
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
typedef struct srvKinect{
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
typedef struct pBuf{
    std::vector<uint8_t> *ptrVideoBuf;//!< container of video info from kinect
    std::vector<uint16_t> *ptrDepthBuf;//!< container of depth info from kinect
    std::vector<point3c> *ptrP3Buf;//!< container of points cloud <- video+depth
    std::vector<point2> *ptrP2Buf;//!< container of 2D points = (point cloud) - color - z
    std::vector<uint32_t> *ptrBarridoBuf;//!< barridoBuf contains distance to closer object on angle (360-i)/2 degrees, xOz plane (horizontal to camera)
    accel *ptrAccel;//!< acceleration components x,y,z (y ~ 9,81 if m_iAnguloKinect=0)
}pBuf;

/*!
 * \brief to easy locate variables in vectors
 *
 * In server it contains time; but in client it's used
 * for frequency control.
 */
enum eOption {
    e_video,
    e_depth,
    e_3,
    e_2,
    e_barrido,
    e_accel,
    e_loop,
    e_xtra
};

#endif // TYPEKINECT_H
