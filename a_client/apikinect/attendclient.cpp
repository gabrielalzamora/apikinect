/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <vector>
#include <QTimer>
#include <QDataStream>
#include "attendclient.h"

/*!
 * \class AttendClient
 * \brief is created for each client connected to server.
 *
 * Handle all comunications between server and client
 */

/*!
 * \brief constructor
 * \param socket
 * \param ptrToBuffers
 * \param parent
 */
AttendClient::AttendClient(QTcpSocket *socket, pBuf *ptrToBuffers, QObject *parent) : QObject(parent)
{
    qDebug("AttendClient::AttendClient");
    peerAddr = socket->peerAddress();
    peerPort = socket->peerPort();
    m_socket = socket;
    connect(m_socket,SIGNAL(readyRead()),this,SLOT(readSrvKdata()));
    qDebug() << "  peerAddr = " << peerAddr.toString();
    qDebug("  peerPort = %u",peerPort);

    structBuffers.ptrVideoBuf=ptrToBuffers->ptrVideoBuf;
    structBuffers.ptrDepthBuf=ptrToBuffers->ptrDepthBuf;
    structBuffers.ptrP3Buf = ptrToBuffers->ptrP3Buf;
    structBuffers.ptrP2Buf = ptrToBuffers->ptrP2Buf;
    structBuffers.ptrBarridoBuf = ptrToBuffers->ptrBarridoBuf;
    structBuffers.ptrAccel = ptrToBuffers->ptrAccel;

    startServers();
}
/*!
 * \brief destructor
 */
AttendClient::~AttendClient()
{
    //qDebug("AttendClient::~AttendClient");
    m_socket->deleteLater();
    s_video->deleteLater();
    s_depth->deleteLater();
    s_3d->deleteLater();
    s_2d->deleteLater();
    s_barrido->deleteLater();
    s_accel->deleteLater();

}
/*!
 * \brief create servers to listen client
 */
void AttendClient::startServers()
{
    //qDebug("AttendClient::startServers");
    s_video = new QTcpServer(this);
    s_depth = new QTcpServer(this);
    s_3d = new QTcpServer(this);
    s_2d = new QTcpServer(this);
    s_barrido = new QTcpServer(this);
    s_accel = new QTcpServer(this);

    s_video->listen(QHostAddress::Any,VIDEOPORT);
    connect(s_video,SIGNAL(newConnection()),this,SLOT(videoConnection()));
    s_depth->listen(QHostAddress::Any,DEPTHPORT);
    connect(s_depth,SIGNAL(newConnection()),this,SLOT(depthConnection()));
    s_3d->listen(QHostAddress::Any,THREEPORT);
    connect(s_3d,SIGNAL(newConnection()),this,SLOT(connection3D()));
    s_2d->listen(QHostAddress::Any,TWOPORT);
    connect(s_2d,SIGNAL(newConnection()),this,SLOT(connection2D()));
    s_barrido->listen(QHostAddress::Any,BARRIDOPORT);
    connect(s_barrido,SIGNAL(newConnection()),this,SLOT(connectionBarrido()));
    s_accel->listen(QHostAddress::Any,ACCELPORT);
    connect(s_accel,SIGNAL(newConnection()),this,SLOT(incomingAccel()));

    sizeSrvK = sizeVideo = sizeDepth = size3d = size2d = sizeBarrido = sizeAccel = 0;
}
/*!
 * \brief read srvKinect new value when sended by client
 */
void AttendClient::readSrvKdata()
{
    //qDebug("AttendClient::readSrvKdata");

    QDataStream in(m_socket);
    in.setVersion(QDataStream::Qt_5_0);

    if (sizeSrvK == 0) {
        if (m_socket->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeSrvK;
    }
    if (m_socket->bytesAvailable() < sizeSrvK){
        return;
    }

    in >> flagSrvK;
    if( !flagSrvK ){
        m_socket->disconnectFromHost();
        qDebug("Cliente ordena desconectar");
        this->deleteLater();
        this->~AttendClient();
        return;
    }
    in >> srvK.m_fAngulo;
    in >> srvK.m_iAnguloKinect;
    in >> srvK.m_fAltura;
    in >> srvK.m_fYMin;
    in >> srvK.m_fYMax;
    in >> srvK.m_fZMax;
    in >> srvK.m_ulRefresco3D;
    in >> srvK.m_usModulo3D;
    in >> srvK.m_bEnvio3D;
    in >> srvK.m_bEnvio2D;
    in >> srvK.m_bEnvioBarrido;
    in >> srvK.m_bCompress3D;
    in >> srvK.m_iBarridoEcu;
    in >> srvK.m_iBarridoYMin;
    in >> srvK.m_iBarridoYMax;
    in >> srvK.m_ulRefrescoDepth;
    in >> srvK.m_bEnvioDepth;
    in >> srvK.m_bCompressDepth;
    in >> srvK.m_ulRefrescoColor;
    in >> srvK.m_bEnvioColor;
    in >> srvK.m_bCompressColor;
    emit newSrvKinect(srvK);
    sizeSrvK = 0;
}
/*!
 * \brief send SrvKinect data to client and store it in AttendClient
 * \param newSrvK
 */
void AttendClient::sendSrvKinect(srvKinect newSrvK)
{
    //qDebug("AttendClient::sendSrvKinect");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << quint64(0);
    out << (srvK.m_fAngulo = newSrvK.m_fAngulo);
    out << (srvK.m_iAnguloKinect = newSrvK.m_iAnguloKinect);
    out << (srvK.m_fAltura = newSrvK.m_fAltura);
    out << (srvK.m_fYMin = newSrvK.m_fYMin);
    out << (srvK.m_fYMax = newSrvK.m_fYMax);
    out << (srvK.m_fZMax = newSrvK.m_fZMax);
    out << (srvK.m_ulRefresco3D = newSrvK.m_ulRefresco3D);
    out << (srvK.m_usModulo3D = newSrvK.m_usModulo3D);
    out << (srvK.m_bEnvio3D = newSrvK.m_bEnvio3D);
    out << (srvK.m_bEnvio2D = newSrvK.m_bEnvio2D);
    out << (srvK.m_bEnvioBarrido = newSrvK.m_bEnvioBarrido);
    out << (srvK.m_bCompress3D = newSrvK.m_bCompress3D);
    out << (srvK.m_iBarridoEcu = newSrvK.m_iBarridoEcu);
    out << (srvK.m_iBarridoYMin = newSrvK.m_iBarridoYMin);
    out << (srvK.m_iBarridoYMax = newSrvK.m_iBarridoYMax);
    out << (srvK.m_ulRefrescoDepth = newSrvK.m_ulRefrescoDepth);
    out << (srvK.m_bEnvioDepth = newSrvK.m_bEnvioDepth);
    out << (srvK.m_bCompressDepth = newSrvK.m_bCompressDepth);
    out << (srvK.m_ulRefrescoColor = newSrvK.m_ulRefrescoColor);
    out << (srvK.m_bEnvioColor = newSrvK.m_bEnvioColor);
    out << (srvK.m_bCompressColor = newSrvK.m_bCompressColor);

    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    m_socket->write(buff);
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O video data
 */
void AttendClient::videoConnection()
{
    //qDebug("AttendClient::videoConnection");
    skt_video = s_video->nextPendingConnection();
    connect(skt_video,SIGNAL(readyRead()),this,SLOT(videoIncoming()));
    t_video.start();
}
/*!
 * \brief read client incomming message and call videoRefresh()
 */
void AttendClient::videoIncoming()
{
    //qDebug("AttendClient::videoIncoming");
    if(skt_video->peerAddress() != peerAddr)
        return;//if client is not our client wait for next connection

    QDataStream in(skt_video);
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeVideo == 0) {
        if (skt_video->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeVideo;
    }
    if (skt_video->bytesAvailable() < sizeVideo){
        return;
    }
    in >> flagVideo;

    sizeVideo = 0;

    if( !flagVideo ){
        skt_video->disconnectFromHost();
        return;
    }
    videoRefresh();
}
/*!
 * \brief control time between sended video images (refresco)
 */
void AttendClient::videoRefresh()
{
    //qDebug("AttendClient::videoRefresh");
    int duration = srvK.m_ulRefrescoColor - t_video.elapsed();

    if( duration > 0 ){
        QTimer::singleShot(duration,this,SLOT(videoSend()));
    }else{
        videoSend();
    }
}
/*!
 * \brief send video frame (image) through skt_video to client
 */
void AttendClient::videoSend()
{
    //qDebug("AttendClient::videoSend");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0);

    for(int i = 0; i < RES_KINECT_3D_W*RES_KINECT_VIDEO_H*3; i++){
        out << (*structBuffers.ptrVideoBuf)[i];
    }
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));
    skt_video->write(buff);

    t_video.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O depth data
 */
void AttendClient::depthConnection()
{
    //qDebug("AttendClient::depthConnection");
    skt_depth = s_depth->nextPendingConnection();
    connect(skt_depth,SIGNAL(readyRead()),this,SLOT(depthIncoming()));
    t_depth.start();
}
/*!
 * \brief read client incomming message and call depthRefresh()
 */
void AttendClient::depthIncoming()
{
    //qDebug("AttendClient::depthIncoming");
    if(skt_depth->peerAddress() != peerAddr)
        return;

    QDataStream in(skt_depth);
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeDepth == 0) {
        if (skt_depth->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeDepth;
    }
    if (skt_depth->bytesAvailable() < sizeDepth){
        return;
    }

    in >> flagDepth;

    sizeDepth = 0;

    if( !flagDepth ){
        skt_depth->disconnectFromHost();
        return;
    }
    depthRefresh();
}
/*!
 * \brief control time between sended depth images (refresco)
 */
void AttendClient::depthRefresh()
{
    //qDebug("AttendClient::depthRefresh");
    int duration = srvK.m_ulRefrescoDepth - t_depth.elapsed();

    if( duration > 0 ){
        QTimer::singleShot(duration,this,SLOT(depthSend()));
    }else{
        depthSend();
    }
}
/*!
 * \brief send depth frame (image) through skt_depth to client
 */
void AttendClient::depthSend()
{
    //qDebug("AttendClient::sendDepth");

    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    int sizeToSend;
    sizeToSend = RES_KINECT_3D_W*RES_KINECT_VIDEO_H*sizeof((*structBuffers.ptrDepthBuf)[0]);

    out << quint64(0);
    for(int i = 0; i < RES_KINECT_3D_W*RES_KINECT_VIDEO_H; i++){
        out << (*structBuffers.ptrDepthBuf)[i];
    }
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_depth->write(buff);

    t_depth.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O  3d data
 */
void AttendClient::connection3D()
{
    //qDebug("AttendClient::connection3D");
    skt_3d = s_3d->nextPendingConnection();
    connect(skt_3d,SIGNAL(readyRead()),this,SLOT(incoming3D()));
    t_3d.start();
}
/*!
 * \brief read client incomming message and call refresh3D()
 */
void AttendClient::incoming3D()
{
    //qDebug("AttendClient::incoming3D");
    if(skt_3d->peerAddress() != peerAddr)
        return;

    QDataStream in(skt_3d);
    in.setVersion(QDataStream::Qt_5_0);
    if (size3d == 0) {
        if (skt_3d->bytesAvailable() < sizeof(quint64))
            return;
        in >> size3d;
    }
    if (skt_3d->bytesAvailable() < size3d){
        return;
    }
    in >> flag3d;

    size3d = 0;

    if( !flag3d ){
        skt_3d->disconnectFromHost();
        return;
    }
    refresh3D();
}
/*!
 * \brief control time between sended 3d images (refresco)
 */
void AttendClient::refresh3D()
{
    //qDebug("AttendClient::refresh3D");
    int duration = srvK.m_ulRefresco3D - t_3d.elapsed();

    if( duration > 0 ){
        QTimer::singleShot(duration,this,SLOT(send3D()));
    }else{
        send3D();
    }
}
/*!
 * \brief send 3D point cloud through skt_3d to client
 */
void AttendClient::send3D()
{
    //qDebug("AttendClient::send3D");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << qint64(0);

    for(uint i = 0; i<(*structBuffers.ptrP3Buf).size();i++){
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].x;//read one point3c x dimension
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].y;
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].z;
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbRed;//read point3c color red component
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbGreen;
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbBlue;
    }
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_3d->write(buff);

    t_3d.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O 2d data
 */
void AttendClient::connection2D()
{
    //qDebug("AttendClient::connection2D");
    skt_2d = s_2d->nextPendingConnection();
    connect(skt_2d,SIGNAL(readyRead()),this,SLOT(incoming2D()));
    t_2d.start();
}
/*!
 * \brief read client incomming message and call refresh2D()
 */
void AttendClient::incoming2D()
{
    //qDebug("AttendClient::incoming2D");
    if(skt_2d->peerAddress() != peerAddr)
        return;

    QDataStream in(skt_2d);
    in.setVersion(QDataStream::Qt_5_0);
    if (size2d == 0) {
        if (skt_2d->bytesAvailable() < sizeof(quint64))
            return;
        in >> size2d;
    }
    if (skt_2d->bytesAvailable() < size2d){
        return;
    }
    in >> flag2d;

    size2d = 0;

    if( !flag2d ){
        skt_2d->disconnectFromHost();
        return;
    }

    refresh2D();
}
/*!
 * \brief control time between sended 2d images (refresco)
 */
void AttendClient::refresh2D()
{
    //qDebug("AttendClient::refresh2D");
    int duration = srvK.m_ulRefresco3D - t_2d.elapsed();

    if( duration > 0 ){
        QTimer::singleShot(duration,this,SLOT(send2D()));
    }else{
        send2D();
    }
}
/*!
 * \brief send 2D point cloud through skt_2d to client
 */
void AttendClient::send2D()
{
    //qDebug("AttendClient::send2D");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << qint64(0);

    for(uint i = 0; i<(*structBuffers.ptrP2Buf).size();i++){
        out << (int16_t) (*structBuffers.ptrP2Buf)[i].x;
        out << (int16_t) (*structBuffers.ptrP2Buf)[i].z;
    }
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_2d->write(buff);

    t_2d.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O barrido data
 */
void AttendClient::connectionBarrido()
{
    //qDebug("AttendClient::connectionBarrido");
    skt_barrido = s_barrido->nextPendingConnection();
    connect(skt_barrido,SIGNAL(readyRead()),this,SLOT(incomingBarrido()));
    t_barrido.start();
}
/*!
 * \brief read client incomming message and call refreshBarrido()
 */
void AttendClient::incomingBarrido()
{
    //qDebug("AttendClient::incomingBarrido");
    if(skt_barrido->peerAddress() != peerAddr)
        return;

    QDataStream in(skt_barrido);
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeBarrido == 0) {
        if (skt_barrido->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeBarrido;
    }
    if (skt_barrido->bytesAvailable() < sizeBarrido){
        return;
    }
    in >> flagBarrido;

    sizeBarrido = 0;

    if( !flagBarrido ){
        skt_barrido->disconnectFromHost();
        return;
    }

    refreshBarrido();
}
/*!
 * \brief control time between sended barrido vector (refresco)
 */
void AttendClient::refreshBarrido()
{
    //qDebug("AttendClient::refreshBarrido");
    int duration = srvK.m_ulRefresco3D - t_barrido.elapsed();

    if( duration > 0 ){
        QTimer::singleShot(duration,this,SLOT(sendBarrido()));
    }else{
        sendBarrido();
    }
}
/*!
 * \brief send barrido (vector) through skt_barrido to client
 */
void AttendClient::sendBarrido()
{
    //qDebug("AttendClient::sendBarrido");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0);
    for( int i = 0; i<360; i++ ){
        out << (uint32_t)(*structBuffers.ptrBarridoBuf)[i];
    }
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_barrido->write(buff);

    t_barrido.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O data
 */
void AttendClient::incomingAccel()
{
    //qDebug("AttendClient::incomingAccel");
    skt_accel = s_accel->nextPendingConnection();
    connect(skt_accel,SIGNAL(readyRead()),this,SLOT(sendAccel()));
}
/*!
 * \brief send accel struct through skt_accel to client
 */
void AttendClient::sendAccel()
{
    //qDebug("AttendClient::sendAccel");
    if(skt_accel->peerAddress() != peerAddr){
        return;
    }

    QDataStream in(skt_accel);
    in.setVersion(QDataStream::Qt_5_0);

    if (sizeAccel == 0) {
        if (skt_accel->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeAccel;
    }
    if (skt_accel->bytesAvailable() < sizeAccel){
        return;
    }

    in >> flagAccel;

    sizeAccel = 0;

    if( !flagAccel ){
        skt_accel->disconnectFromHost();
        return;
    }

    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    out << qint64(0);
    out << (double) (*structBuffers.ptrAccel).accel_x;
    out << (double) (*structBuffers.ptrAccel).accel_y;
    out << (double) (*structBuffers.ptrAccel).accel_z;
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));

    skt_accel->write(buff);
}
