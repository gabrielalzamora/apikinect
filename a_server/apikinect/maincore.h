/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef MainCore_H
#define MainCore_H

#include <QTcpServer>
#include "apikinect.h"
#include "attendclient.h"
#include "framegl.h"
#include "configdata.h"

namespace Ui {
class MainCore;
}

class MainCore : public QObject
{
    Q_OBJECT

public:
    explicit MainCore(QObject *parent = 0);
    ~MainCore();
    std::vector<uint8_t> videoBuf;//!< container of video info from kinect
    std::vector<uint16_t> depthBuf;//!< container of depth info from kinect
    std::vector<point3rgb> p3rgbBuf;//!< container of points cloud <- video+depth
    std::vector<point2> p2Buf;//!< container of 2D points = (point cloud) - color - y(coordinate)
    std::vector<uint32_t> barridoBuf;//!< barridoBuf contains distance on angle (360-i)/2 degrees, xOz plane

signals:
    void printVideo();//warn videoBuf ready to be printed
    void printDepth();
    void printBarrido();
    void print3D();
    void print2D();
    void printTimeVector();

public slots:
    //freenect kinect
    void startK(int indexK=0);
    void stopK(int indexK=0);
    void updateKinect();//send current ledOption and angle to kinect
    void updateSrvKinect(srvKinect newSrvK);//set new configuration data
    //setters & getters
    void setCurrentKIndex(int index);
    int getCurrentKIndex();
    int getKnumber();
    int getTime(eOption opt);//in milliseconds
    accel getAccel();

private slots:
    void init();
    //server
    void startServer();
    void attendNewClient();

protected:


private:
    int numKinects;//!< number of detected kinects
    int currentKIndex;//!< index of active kinect

    Freenect::Freenect freenect;//!< Freenect class object to start events thread and Apikinect
    freenect_context *context;//!< point to usb context associated to kinect data handling
    Apikinect *device;//!< class object that handle kinect sending led, angle orders, receiving frames, acceleration data
    ConfigData *config;
    QTcpServer *server;
    AttendClient *attendant;
    std::vector<AttendClient*> attendVector;//!< contain active AttendClient (allow to access them)

    accel a;//!< acceleration components x,y,z (y ~ 9,81 if m_iAnguloKinect=0)
    std::vector<int> timeVector;//msecs
    pBuf structBuffers;//!< to tell server where to find data buffers

    int flag;//!< 0 stop loop(), otherwise let loop() run

};
#endif // MainCore_H
