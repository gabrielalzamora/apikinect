/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINCLIENT_H
#define MAINCLIENT_H

#include <QDataStream>
#include <QObject>
#include <QTime>
#include <QTcpSocket>
#include <QHostAddress>
#include "configdata.h"
#include "typekinect.h"

class MainClient : public QObject
{
    Q_OBJECT

public:
    explicit MainClient(QObject *parent = 0);
    ~MainClient();

signals:
    void printVideo();//warn video image ready to be printed
    void printDepth();
    void printBarrido();
    void print3D();
    void print2D();
    void printAccel();
    void printTimeVector();

    void socketErrorSignal(bool status);
    void socketErrorSignalVideo(bool status);
    void socketErrorSignalDepth(bool status);
    void socketErrorSignal3D(bool status);
    void socketErrorSignal2D(bool status);
    void socketErrorSignalBarrido(bool status);
    void socketErrorSignalAccel(bool status);

    void updateSrvKinect(srvKinect newSrvK);// tell GUI to update srvKinect data shown
    void actualizeGUIsrvKinect(SrvKinect *newSrvK);//VisorQT specific note first S in SrvKinect not srvKinect
    void sendMessage(QString str);// send String info

public slots:
    void setTime(eOption opt, int msec);
    int getTime(eOption opt);
    void setHost(QString addr);
    void setSrvKinect(srvKinect newSrvK);// update config->srvK with new data
    void setGUISrvKinect(srvKinect newSrvK);//set GUI data here and on server
    srvKinect getSrvKinect();
    accel getAccel();
    void requestNext(QTcpSocket *socket);// request next image/pointsCloud/accel... to server
    void requestStop(QTcpSocket *socket);//request to server disconnect socket
    //config srvKinect
    void initConnection();
    void closeConnection();
    void readSrvKdata();
    void srvKinectToServer();// send current config->srvK to server
    void socketErrorSrvKinect(QAbstractSocket::SocketError socketError);
    //video
    void initVideo();
    void finalizeVideo();
    void readDataVideo();
    void socketErrorVideo(QAbstractSocket::SocketError socketError);
    //depth
    void initDepth();
    void finalizeDepth();
    void readDataDepth();
    void socketErrorDepth(QAbstractSocket::SocketError socketError);
    //3D
    void init3D();
    void finalize3D();
    void readData3D();
    void socketError3D(QAbstractSocket::SocketError socketError);
    //2D
    void init2D();
    void finalize2D();
    void readData2D();
    void socketError2D(QAbstractSocket::SocketError socketError);
    //barrido
    void initBarrido();
    void finalizeBarrido();
    void readDataBarrido();
    void socketErrorBarrido(QAbstractSocket::SocketError socketError);
    //accel
    void initAccel();
    void finalizeAccel();
    void readDataAccel();
    void socketErrorAccel(QAbstractSocket::SocketError socketError);
    void showAccel(accel a);

public:
    ConfigData *config;
    std::vector<int> timeVector;//msecs
    QTime t_video, t_depth, t_3D, t_2D, t_barrido;
    //main connection
    QHostAddress hostAddr;
    QTcpSocket *skt_config;
    int connectedServer;
    quint64 sizeConfig;
    //Video
    QTcpSocket *skt_video;
    int connectedVideo;
    quint64 sizeVideo;
    std::vector<uint8_t> videoBuf;
    //Depth
    QTcpSocket *skt_depth;
    int connectedDepth;
    quint64 sizeDepth;
    std::vector<uint16_t> depthBuf;
    //3D
    QTcpSocket *skt_3D;
    int connected3D;
    quint64 size3D;
    std::vector<point3rgb> p3Buf;
    //2D
    QTcpSocket *skt_2D;
    int connected2D;
    quint64 size2D;
    std::vector<point2> p2Buf;
    //Barrido
    QTcpSocket *skt_barrido;
    int connectedBarrido;
    quint64 sizeBarrido;
    std::vector<uint32_t> barridoBuf;
    //Accel
    QTcpSocket *skt_accel;
    int connectedAccel;
    quint64 sizeAccel;
    accel acceleration;

private:
    void initMainClient();
    void initConnects();

    void showK(srvKinect srvK);//-----DEBUG
};

#endif // MAINCLIENT_H
