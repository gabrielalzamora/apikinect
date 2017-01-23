/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINSERVER_H
#define MAINSERVER_H

class QTcpServer;
class QTimer;
class Apikinect;
class AttendClient;
class ConfigData;
#include "apikinect.h"

namespace Ui {
class MainServer;
}

class MainServer : public QObject
{
    Q_OBJECT

public:
    explicit MainServer(bool sirvo = true, QObject *parent = 0);
    ~MainServer();
    std::vector<uint8_t> videoBuf;//!< container of video info from kinect
    std::vector<uint16_t> depthBuf;//!< container of depth info from kinect
    std::vector<point3rgb> p3rgbBuf;//!< container of points cloud <- video+depth
    //std::vector<point3c> p3cBuf;//!< container of points cloud with transparency component
    std::vector<point2> p2Buf;//!< container of 2D points = (point cloud) - color - y(coordinate)
    std::vector<uint32_t> barridoBuf;//!< barridoBuf contains distance on angle (360-i)/2 degrees, xOz plane

signals:
    void printVideo();//warn videoBuf ready to be printed
    void printDepth();
    void printBarrido();
    void print3D();
    void print2D();
    void printTimeVector();

    void updateSrvKinect(srvKinect newSrvK);
    void updateClientSrvKinect(srvKinect newSrvK);

public slots:
    //freenect kinect
    void startK(int indexK=0);
    void stopK(int indexK=0);
    void updateKinect();//send current ledOption and angle to kinect
    //getters & setters
    QString getServerIp();
    int getDeviceStatus();
    void setSrvKinect(srvKinect newSrvK);//set new configuration data
    void setGUISrvKinect(srvKinect newSrvK);//set GUI data here and on clients
    srvKinect getSrvKinect();
    void setLed(int ledOpt);
    int getLed();
    void setIR(bool irOpt);
    bool getIR();
    void setCurrentKIndex(int index);
    int getCurrentKIndex();
    int getKnumber();
    int getTime(eOption opt);//in milliseconds : e_video, e_depth, e_3, e_2, e_barrido, e_accel
    void setTime(eOption opt,int msec);
    accel getAccel();
    //start timers
    void go();// start sending data to mainwindow acord to configdata info
    void stop();// stop sending data to mainwindow

private slots:
    void init();
    int depthVoid();
    //server
    void startServer();
    void attendNewClient();
    //
    void nextVideoFrame();// convenience function to use qtimers
    void nextDepthFrame();// convenience function to use qtimers
    void next3DFrame();// convenience function to use qtimers
    void nextTimeVector();// convenience function to use qtimers

protected:


private:
    int numKinects;// number of detected kinects
    int currentKIndex;// index of active kinect
    QTimer *timerVideo;
    QTimer *timerDepth;
    QTimer *timer3D;
    QTimer *timerTime;

    Freenect::Freenect freenect;// Freenect class object to start events thread and Apikinect
    freenect_context *context;// point to usb context associated to kinect data handling
    Apikinect *device;// object that handle kinect sending led, angle orders, receiving frames, acceleration data
    ConfigData *config;// store config data as led, Kinect angle, refresh rate, what to send...
    QTcpServer *server;
    AttendClient *attendant;// in charge of last connected client
    std::vector<AttendClient*> attendVector;// contain active AttendClient (allow to access them)

    accel a;// acceleration components x,y,z (y ~ 9,81 if m_iAnguloKinect=0)
    std::vector<int> timeVector;//msecs
    pBuf structBuffers;// to tell AttendClient *attendant where to find data buffers

    void showK(srvKinect srvK);///------------------------------------------------------DEBUG
};
#endif // MAINSERVER_H
