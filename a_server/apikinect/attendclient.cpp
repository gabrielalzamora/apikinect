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
    if(s_video->isListening()){
        if(skt_video->isValid()){
            skt_video->disconnectFromHost();
            if( !skt_video->waitForDisconnected(3000) ){
                skt_video->abort();
            }
        }
        s_video->disconnect();
    }
    if(s_depth->isListening()){
        if(skt_depth->isValid()){
            skt_depth->disconnectFromHost();
            if( !skt_depth->waitForDisconnected(3000) ){
                skt_depth->abort();
            }
        }
        s_depth->disconnect();
    }
    if(s_3d->isListening()){
        if(skt_3d->isValid()){
            skt_3d->disconnectFromHost();
            if( !skt_3d->waitForDisconnected(3000) ){
                skt_3d->abort();
            }
        }
        s_3d->disconnect();
    }
    if(s_2d->isListening()){
        if(skt_2d->isValid()){
            skt_2d->disconnectFromHost();
            if( !skt_2d->waitForDisconnected(3000) ){
                skt_2d->abort();
            }
        }
        s_2d->disconnect();
    }
    if(s_barrido->isListening()){
        if(skt_barrido->isValid()){
            skt_barrido->disconnectFromHost();
            if( !skt_barrido->waitForDisconnected(3000) ){
                skt_barrido->abort();
            }
        }
        s_barrido->disconnect();
    }
    if(s_accel->isListening()){
        if(skt_accel->isValid()){
            skt_accel->disconnectFromHost();
            if( !skt_accel->waitForDisconnected(3000) ){
                skt_accel->abort();
            }
        }
        s_accel->disconnect();
    }
}
/*!
 * \brief create servers to listen client
 */
void AttendClient::startServers()
{
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
    //-------------------------------------------------------read client msg
    QDataStream in(m_socket);//read client petition
    in.setVersion(QDataStream::Qt_5_0);

    if (sizeSrvK == 0) {//check there's enough bytes to read
        if (m_socket->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeSrvK;//read and save into quint64 sizeSrvK
    }
    if (m_socket->bytesAvailable() < sizeSrvK){//check there's enough bytes to read
        return;//if not wait till you have all data size video says
    }

    in >> flagSrvK;//flag = 0 will stop and disconnect, != 0 read srvK
    //qDebug("tamaño: %llu  info: %u", sizeSrvK, flagSrvK);//DEBUG
    if( !flagSrvK ){
        m_socket->disconnectFromHost();
        qDebug("Cliente ordena desconectar");
        this->deleteLater();
        this->~AttendClient();
        return;
    }//--------------- only goes ahead if all data received & flagSrvK != 0
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
    sizeSrvK = 0;//to allow reading next message size
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
    //-------------------------------------------------------read client msg
    QDataStream in(skt_video);//read client petition
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeVideo == 0) {//check there's enough bytes to read
        if (skt_video->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeVideo;//read and save into quint64 sizeVideo
    }
    if (skt_video->bytesAvailable() < sizeVideo){//check there's enough bytes to read
        return;//if not wait till you have all data sizeVideo says
    }
    in >> flagVideo;//read flag = 0 will stop and disconnect, != 0 send image
    //qDebug("tamaño: %llu  info: %u", sizeVideo, flagVideo);//DEBUG
    sizeVideo = 0;//to allow reading next message size

    if( !flagVideo ){
        skt_video->disconnectFromHost();
        //qDebug("Cliente ordena desconectar video");
        return;
    }//--------------- only goes ahead if all data received & flagVideo != 0
    videoRefresh();
}
/*!
 * \brief control time between sended video images (refresco)
 */
void AttendClient::videoRefresh()
{
    //qDebug("AttendClient::videoRefresh");
    int duration = srvK.m_ulRefrescoColor - t_video.elapsed();
    //qDebug("  antes de retraso %i",t_video.elapsed());

    if( duration > 0 ){
        //qDebug("  CON retraso duracion= %i",duration);
        QTimer::singleShot(duration,this,SLOT(videoSend()));
        //qDebug("  En retraso tiempoooo  %i",t_video.elapsed());
    }else{
        //qDebug("  enviamos SIN retraso %i",t_video.elapsed());
        videoSend();//enviamos
        //qDebug("  YA enviado %i",t_video.elapsed());
    }
}
/*!
 * \brief send video frame (image) through skt_video to client
 */
void AttendClient::videoSend()
{
    //qDebug("AttendClient::videoSend");
    QImage image = QImage(structBuffers.ptrVideoBuf->data(),640,480,QImage::Format_RGB888);
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << image;
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//set buff size at beginning
//    qDebug("  tamaño antes enviado: %u",(buff.size()-sizeof(quint64))) ;//DEBUG
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
    t_depth.start();//to have first time reference
}
/*!
 * \brief read client incomming message and call depthRefresh()
 */
void AttendClient::depthIncoming()
{
    //qDebug("AttendClient::depthIncoming");
    if(skt_depth->peerAddress() != peerAddr)
        return;//if client is not our client wait for next connection
    //-------------------------------------------------------read client msg
    QDataStream in(skt_depth);
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeDepth == 0) {//check there's enough bytes to read
        if (skt_depth->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeDepth;//read and save into quint64 sizeDepth
    }
    if (skt_depth->bytesAvailable() < sizeDepth){//check there's enough bytes to read
        return;//if not wait till you have all data sizeDepth says
    }
    in >> flagDepth;//read flag = 0 will stop and disconnect, != 0 send image
    //qDebug("tamaño: %llu  info: %u", sizeDepth, flagDepth);//DEBUG
    sizeDepth = 0;//to allow reading next message size

    if( !flagDepth ){
        skt_depth->disconnectFromHost();
        //qDebug("Cliente ordena desconectar depth");
        return;
    }//--------------- only goes ahead if all data received & flagDepth != 0
    depthRefresh();
}
/*!
 * \brief control time between sended depth images (refresco)
 */
void AttendClient::depthRefresh()
{
    //qDebug("AttendClient::depthRefresh");
    int duration = srvK.m_ulRefrescoDepth - t_depth.elapsed();
    //qDebug("  antes de retraso %i",t_depth.elapsed());

    if( duration > 0 ){
        //qDebug("  CON retraso duracion= %i",duration);
        QTimer::singleShot(duration,this,SLOT(depthSend()));
        //qDebug("  En retraso tiempoooo  %i",t_depth.elapsed());
    }else{
        //qDebug("  enviamos SIN retraso %i",t_depth.elapsed());
        depthSend();//enviamos
        //qDebug("  YA enviado %i",t_depth.elapsed());
    }
}
/*!
 * \brief send depth frame (image) through skt_depth to client
 */
void AttendClient::depthSend()
{
    //qDebug("AttendClient::sendDepth");
    QImage image = QImage(640,480,QImage::Format_Grayscale8);
    unsigned char r,g,b, distaChar;
    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
            int value = (*structBuffers.ptrDepthBuf)[(x+y*640)];//value is distance in mm
            distaChar = value/39;//to transform distance to 8bit grey
            r=g=b=distaChar;
            image.setPixel(x,y,qRgb(r,g,b));
        }
    }
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << image;
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//set buff size at beginning
//    qDebug("  tamaño antes enviado: %u",(buff.size()-sizeof(quint64))) ;//DEBUG
    skt_depth->write(buff);//enviamos
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
        return;//if client is not our client wait for next connection
    //-------------------------------------------------------read client msg
    QDataStream in(skt_3d);
    in.setVersion(QDataStream::Qt_5_0);
    if (size3d == 0) {//check there's enough bytes to read
        if (skt_3d->bytesAvailable() < sizeof(quint64))
            return;
        in >> size3d;//read and save into quint64 size3d
    }
    if (skt_3d->bytesAvailable() < size3d){//check there's enough bytes to read
        return;//if not wait till you have all data size3d says
    }
    in >> flag3d;//read flag = 0 will stop and disconnect, != 0 send point cloud
    //qDebug("tamaño: %llu  info: %u", size3d, flag3d);//DEBUG
    size3d = 0;//to allow reading next message size

    if( !flag3d ){
        skt_3d->disconnectFromHost();
        //qDebug("Cliente ordena desconectar 3d");
        return;
    }//--------------- only goes ahead if all data received & flag3d != 0
    refresh3D();
}
/*!
 * \brief control time between sended 3d images (refresco)
 */
void AttendClient::refresh3D()
{
    //qDebug("AttendClient::refresh3D");
    int duration = srvK.m_ulRefresco3D - t_3d.elapsed();
    //qDebug("  antes de retraso %i",t_3d.elapsed());

    if( duration > 0 ){
        //qDebug("  CON retraso duracion= %i",duration);
        QTimer::singleShot(duration,this,SLOT(send3D()));
        //qDebug("  En retraso tiempoooo  %i",t_3d.elapsed());
    }else{
        //qDebug("  enviamos SIN retraso %i",t_3d.elapsed());
        send3D();//enviamos
        //qDebug("  YA enviado %i",t_3d.elapsed());
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
    //qDebug("  tamaño vector = %lu",(*structBuffers.ptrP3Buf).size());

    out << qint64(0);//let space for size info

    for(uint i = 0; i<(*structBuffers.ptrP3Buf).size();i++){
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].x;//read one point3c x dimension
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].y;
        out << (int16_t) (*structBuffers.ptrP3Buf)[i].z;
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbRed;//read point3c color red component
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbGreen;
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbBlue;
        out << (uint8_t) (*structBuffers.ptrP3Buf)[i].color.rgbReserved;
    }
    out.device()->seek(0);//pointer to buff start
    out << quint64(buff.size() - sizeof(quint64));//write buff size at beginning
    //qDebug("  tamaño antes enviado: %lu",(buff.size()-sizeof(quint64))) ;//DEBUG
    //qDebug("  tamaño vector = %lu",(*structBuffers.ptrP3Buf).size());
    skt_3d->write(buff);//enviamos
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
        return;//if client is not our client wait for next connection

    QDataStream in(skt_2d);
    in.setVersion(QDataStream::Qt_5_0);
    if (size2d == 0) {//check there's enough bytes to read
        if (skt_2d->bytesAvailable() < sizeof(quint64))
            return;
        in >> size2d;//read and save into quint64 size2d
    }
    if (skt_2d->bytesAvailable() < size2d){//check there's enough bytes to read
        return;//if not wait till you have all data size2d says
    }
    in >> flag2d;//read flag = 0 will stop and disconnect, != 0 send point cloud
    //qDebug("tamaño: %llu  info: %u", size2d, flag2d);//DEBUG
    size2d = 0;//to allow reading next message size

    if( !flag2d ){
        skt_2d->disconnectFromHost();
        //qDebug("Cliente ordena desconectar 2d");
        return;
    }//--------------- only goes ahead if all data received & flag2d != 0
    refresh2D();
}
/*!
 * \brief control time between sended 2d images (refresco)
 */
void AttendClient::refresh2D()
{
    //qDebug("AttendClient::refresh2D");
    int duration = srvK.m_ulRefresco3D - t_2d.elapsed();
    //qDebug("  antes de retraso %i",t_2d.elapsed());

    if( duration > 0 ){
        //qDebug("  CON retraso duracion= %i",duration);
        QTimer::singleShot(duration,this,SLOT(send2D()));
        //qDebug("  En retraso tiempoooo  %i",t_2d.elapsed());
    }else{
        //qDebug("  enviamos SIN retraso %i",t_2d.elapsed());
        send2D();//enviamos
        //qDebug("  YA enviado %i",t_2d.elapsed());
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

    out << qint64(0);//let space for size info

    for(uint i = 0; i<(*structBuffers.ptrP2Buf).size();i++){
        out << (int16_t) (*structBuffers.ptrP2Buf)[i].x;//read one point2c x dimension
        out << (int16_t) (*structBuffers.ptrP2Buf)[i].z;
    }
    out.device()->seek(0);//pointer to buff start
    out << quint64(buff.size() - sizeof(quint64));//write buff size at beginning
    //qDebug("  tamaño antes enviado: %lu",(buff.size()-sizeof(quint64))) ;//DEBUG
    //qDebug("  tamaño vector = %lu",(*structBuffers.ptrP2Buf).size());
    skt_2d->write(buff);//enviamos
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

    QDataStream in(skt_barrido);//read client petition
    in.setVersion(QDataStream::Qt_5_0);
    if (sizeBarrido == 0) {//check there's enough bytes to read
        if (skt_barrido->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeBarrido;//read and save into quint64 sizeBarrido
    }
    if (skt_barrido->bytesAvailable() < sizeBarrido){//check there's enough bytes to read
        return;//if not wait till you have all data sizeBarrido says
    }
    in >> flagBarrido;//flag = 0 will stop and disconnect, != 0 send barrido
    //qDebug("tamaño: %llu  info: %u", sizeBarrido, flagBarrido);//DEBUG
    sizeBarrido = 0;//to allow reading next message size

    if( !flagBarrido ){
        skt_barrido->disconnectFromHost();
        //qDebug("Cliente ordena desconectar Barrido");
        return;
    }//--------------- only goes ahead if all data received & flagDepth != 0
    refreshBarrido();
}
/*!
 * \brief control time between sended barrido vector (refresco)
 */
void AttendClient::refreshBarrido()
{
    //qDebug("AttendClient::refreshBarrido");
    int duration = srvK.m_ulRefresco3D - t_barrido.elapsed();
    //qDebug("  antes de retraso %i",t_barrido.elapsed());

    if( duration > 0 ){
        //qDebug("  CON retraso duracion= %i",duration);
        QTimer::singleShot(duration,this,SLOT(sendBarrido()));
        //qDebug("  En retraso tiempoooo  %i",t_2d.elapsed());
    }else{
        //qDebug("  enviamos SIN retraso %i",t_2d.elapsed());
        sendBarrido();//enviamos
        //qDebug("  YA enviado %i",t_2d.elapsed());
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
        //qDebug(" esto es lo que manda barrido[%lu] = %lu",i,(*structBuffers.ptrBarridoBuf)[i]);
    }
    out.device()->seek(0);//point to buff beginning
    out << quint64(buff.size() - sizeof(quint64));//set buff size at beginning
    //qDebug("  tamaño antes enviado: %lu",(buff.size() - sizeof(quint64))) ;//DEBUG
    skt_barrido->write(buff);//send to client
    t_barrido.restart();
}

/*!
 * \brief answer incoming connection; bind to a socket in order to I/O data
 */
void AttendClient::incomingAccel()
{
    skt_accel = s_accel->nextPendingConnection();
    connect(skt_accel,SIGNAL(readyRead()),this,SLOT(sendAccel()));
}
/*!
 * \brief send accel struct through skt_accel to client
 */
void AttendClient::sendAccel()
{
    //qDebug("AttendClient::sendAccel");
    if(skt_accel->peerAddress() != peerAddr){//AttendClient only attend single client
        //qDebug("  otro cliente :o(  %u yo %u ",skt_accel->peerAddress().toIPv4Address(),peerAddr.toIPv4Address());
        return;//if client is not our client wait for next connection
    }

    //-------------------------------------------------------read client msg
    QDataStream in(skt_accel);//read client petition
    in.setVersion(QDataStream::Qt_5_0);

    if (sizeAccel == 0) {//check there's enough bytes to read
        if (skt_accel->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeAccel;//read and save into quint64 size2d
    }
    if (skt_accel->bytesAvailable() < sizeAccel){//check there's enough bytes to read
        return;//if not wait till you have all data size 2D says
    }

    in >> flagAccel;//flag = 0 will stop and disconnect, != 0 send acceleration
    //qDebug("tamaño: %llu  info: %u", size2d, flag2d);//DEBUG
    sizeAccel = 0;//to allow reading next message size

    if( !flagAccel ){
        skt_accel->disconnectFromHost();
        qDebug("Cliente ordena desconectar accel");
        return;
    }//--------------- only goes ahead if all data received & flag2d != 0

//----------------------------------------------------------------create & send 2d vector
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);

    //qDebug("  tamaño vector = %lu",(*structBuffers.ptrP2Buf).size());

    out << qint64(0);//let space for size info
    out << (double) (*structBuffers.ptrAccel).accel_x;
    out << (double) (*structBuffers.ptrAccel).accel_y;
    out << (double) (*structBuffers.ptrAccel).accel_z;
    out.device()->seek(0);//pointer to buff start
    out << quint64(buff.size() - sizeof(quint64));//write buff size at beginning
    //qDebug("  tamaño antes enviado: %lu",(buff.size()-sizeof(quint64))) ;//DEBUG
    //qDebug("  tamaño vector = %lu",sizeof(*structBuffers.ptrAccel));
    skt_accel->write(buff);//enviamos
}
