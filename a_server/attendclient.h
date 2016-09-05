/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef ATTENDCLIENT_H
#define ATTENDCLIENT_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTime>
#include "data.h"

class AttendClient : public QObject
{
    Q_OBJECT
public:
    explicit AttendClient(QTcpSocket *socket, pBuf *ptrToBuffers, QObject *parent=0);
//    explicit AttendClient(QTcpSocket *socket, std::vector<uint8_t> &vectorVideo, QObject *parent=0);//DEBUG
    ~AttendClient();

signals:
    void newSrvKinect(srvKinect newSrvK);

private slots:
    void startServers();
    void readSrvKdata();
    //video
    void videoConnection();
    void videoIncoming();
    void videoRefresh();
    void videoSend();
    //depth
    void depthConnection();
    void depthIncoming();
    void depthRefresh();
    void depthSend();
    //3d
    void connection3D();
    void incoming3D();
    void refresh3D();
    void send3D();
    //2d
    void connection2D();
    void incoming2D();
    void refresh2D();
    void send2D();
    //barrido
    void connectionBarrido();
    void incomingBarrido();
    void refreshBarrido();
    void sendBarrido();
    //accel
    void incomingAccel();
    void sendAccel();

private:
    QTcpSocket *m_socket;//!< as srvK socket
    quint64 sizeSrvK;
    quint8 flagSrvK;
    QHostAddress peerAddr;
    quint16 peerPort;
    pBuf structBuffers;
    srvKinect srvK;

    QTime t_video, t_depth;//!< to control refresh (refresco)
    QTime t_3d, t_2d;
    QTime t_barrido, t_accel;

    //video
    QTcpServer *s_video;
    QTcpSocket *skt_video;
    quint64 sizeVideo;
    quint8 flagVideo;
    //depth
    QTcpServer *s_depth;
    QTcpSocket *skt_depth;
    quint64 sizeDepth;
    quint8 flagDepth;
    //3d
    QTcpServer *s_3d;
    QTcpSocket *skt_3d;
    quint64 size3d;
    quint8 flag3d;
    //2d
    QTcpServer *s_2d;
    QTcpSocket *skt_2d;
    quint64 size2d;
    quint8 flag2d;
    //barrido
    QTcpServer *s_barrido;
    QTcpSocket *skt_barrido;
    quint64 sizeBarrido;
    quint8 flagBarrido;
    //acceleration
    QTcpServer *s_accel;
    QTcpSocket *skt_accel;
    quint64 sizeAccel;
    quint8 flagAccel;
};

#endif // ATTENDCLIENT_H
