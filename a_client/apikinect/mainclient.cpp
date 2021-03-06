/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include "mainclient.h"

/*!
 * \class MainClient
 * \brief Holds Client functionality
 *
 * Through members MainClient, ConfigData (config) and
 * FrameGL (ui->glWidget) all work is done.
 * MainClient handle sockets to comunicate with server and
 *
 * ConfigData (config) holds configuration to handle data.
 * FrameGL (ui->glWidget) all related to 3D view.
 */

/*!
 * \brief constructor
 *
 * MainClient constructor starts sockets and graphics
 * objects to show data on gui. MainWindow get information
 * through MainClient::videoBuf, depthBuf... acceleration
 * \param parent
 */
MainClient::MainClient(QObject *parent) :
    QObject(parent)
{
    initMainClient();
    initConnects();
}
/*!
 * \brief destructor
 * to free memory and close properly
 */
MainClient::~MainClient()
{
    config->deleteLater();
    skt_config->deleteLater();
    skt_video->deleteLater();
    skt_depth->deleteLater();
    skt_3D->deleteLater();
    skt_2D->deleteLater();
    skt_barrido->deleteLater();
    skt_accel->deleteLater();
}

/*!
 * \brief getter for timeVector, return requested time
 * \param[in] opt wich time will return
 * \return requested time in ms
 */
int MainClient::getTime(eOption opt)
{
    //qDebug("MainServer::getTime");
    return timeVector[opt];
}
/*!
 * \brief setter of timeVector.
 * \param opt[in] eOption of timeVector to update.
 * \param msec[in] value to set on eOption.
 */
void MainClient::setTime(eOption opt, int msec)
{
    //qDebug("MainServer::setTime");
    timeVector[opt]=msec;
}
/*!
 * \brief set Host ip
 */
void MainClient::setHost(QString addr)
{
    //qDebug("MainClient::setHost");
    hostAddr.setAddress(addr);
    sendMessage("connect to server ip:");
}
/*!
 * \brief update this->config->srvK with new data
 * \param newSrvK
 */
void MainClient::setSrvKinect(srvKinect newSrvK)
{
    qDebug("MainClient::setSrvKinect");
    config->setSrvK(newSrvK);
    emit updateSrvKinect(newSrvK);
}
/*!
 * \brief if srvKinect changed on GUI save changes and tell to server
 * \param[in] newSrvK
 */
void MainClient::setGUISrvKinect(srvKinect newSrvK)
{
    //qDebug("MainClient::setGUISrvKinect()");
    config->setSrvK(newSrvK);//update current client->config->srvKinect
    srvKinectToServer();//send new srvKinect to server
}
/*!
 * \brief getter for current srvKinect
 * \return current srvKinect in MainServer::ConfigData::srvK
 */
srvKinect MainClient::getSrvKinect()
{
    //qDebug("MainClient::getSrvKinect()");
    return config->getSrvK();
}
/*!
 * \brief MainClient::getAccel
 * \return
 */
accel MainClient::getAccel()
{
    return acceleration;
}
/*!
 * \brief utility function to request new data to socket (in Apikinect way)
 * \param[in] socket to which request new info.
 */
void MainClient::requestNext(QTcpSocket *socket)
{
    //qDebug("MainClient::requestNext()");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    socket->write(buff);
}
/*!
 * \brief utility function to request disconnect to server socket (in Apikinect way)
 * \param [in] socket from which request disconnect.
 */
void MainClient::requestStop(QTcpSocket *socket)
{
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    socket->write(buff);
}

/*!
 * \brief Connect to server and srvKinect update comm.
 */
void MainClient::initConnection()
{
    //qDebug("MainClient::initConnection");
    QString msg;
    msg = "Unable to connect to server\ncheck server ip\n";

    skt_config->connectToHost(hostAddr,SRVKPORT);
    if( !skt_config->waitForConnected(3000) ){
        msg.append(skt_config->errorString());
        sendMessage(msg);
        qDebug("  client NOT connected connectedServer = %d",connectedServer);
        return;
    }
    connectedServer = 1;
    sendMessage("  client connected to server");
    srvKinectToServer();
}
/*!
 * \brief disconnect from server all sockets & close
 */
void MainClient::closeConnection()
{
    //qDebug("MainClient::closeConnection");
    if(skt_config->state()){
        skt_config->disconnectFromHost();
    }
    connectedServer = 0;
}
/*!
 * \brief MainClient::readSrvKdata
 */
void MainClient::readSrvKdata()
{
    qDebug("MainClient::readSrvKdata");
    QDataStream in(skt_config);
    in.setVersion(QDataStream::Qt_5_0);

    if (sizeConfig == 0) {
        if (skt_config->bytesAvailable() < sizeof(quint64))
            return;
        in >> sizeConfig;
    }
    if (skt_config->bytesAvailable() < sizeConfig)
        return;

    srvKinect srvK;
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

    setSrvKinect(srvK);

    if(skt_config->bytesAvailable()){
        qDebug("MainClient::readSrvKdata quedan cosas sin leer en el buffer");//DEBUG
        QByteArray ba;
        ba = skt_config->readAll();
    }

    sizeConfig = 0;//to allow reading next message size
}
/*!
 * \brief send new srvKinect to server
 */
void MainClient::srvKinectToServer()
{
    //qDebug("MainClient::srvKinectToServer");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out << config->getSrvK().m_fAngulo;
    out << config->getSrvK().m_iAnguloKinect;
    out << config->getSrvK().m_fAltura;
    out << config->getSrvK().m_fYMin;
    out << config->getSrvK().m_fYMax;
    out << config->getSrvK().m_fZMax;
    out << config->getSrvK().m_ulRefresco3D;
    out << config->getSrvK().m_usModulo3D;
    out << config->getSrvK().m_bEnvio3D;
    out << config->getSrvK().m_bEnvio2D;
    out << config->getSrvK().m_bEnvioBarrido;
    out << config->getSrvK().m_bCompress3D;
    out << config->getSrvK().m_iBarridoEcu;
    out << config->getSrvK().m_iBarridoYMin;
    out << config->getSrvK().m_iBarridoYMax;
    out << config->getSrvK().m_ulRefrescoDepth;
    out << config->getSrvK().m_bEnvioDepth;
    out << config->getSrvK().m_bCompressDepth;
    out << config->getSrvK().m_ulRefrescoColor;
    out << config->getSrvK().m_bEnvioColor;
    out << config->getSrvK().m_bCompressColor;
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));

    skt_config->write(buff);
}
/*!
 * \brief tell GUI connection error and close all sockets
 * \param socketError
 */
void MainClient::socketErrorSrvKinect(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketError");
    sendMessage("Error de conexión socket config/srvKinect");
    closeConnection();
    emit socketErrorSignal(false);
}

/*!
 * \brief start/stop video connection to server
 *
 * clicked twice stops video
 */
void MainClient::initVideo()
{
    //qDebug("MainClient::initVideo");
    if( connectedVideo ){
        finalizeVideo();
        sendMessage("- video closed ");
        return;
    }else{
        skt_video->connectToHost(hostAddr,VIDEOPORT);
        if( !skt_video->waitForConnected(3000) ){
            sendMessage(skt_video->errorString());
            qDebug("  client VIDEO NOT connected = %d",connectedVideo);
            return;
        }
        connectedVideo = 1;
        sendMessage("- video connected");
    }
    sizeVideo = 0;
    requestNext(skt_video);
    t_video.start();
}
/*!
 * \brief stop video connection to server
 */
void MainClient::finalizeVideo()
{
    //qDebug("MainClient::finalizeVideo");
    if(skt_video->state()){
        setTime(e_video,0);
        requestStop(skt_video);//request STOP video, to server
        skt_video->disconnectFromHost();
    }
    connectedVideo = 0;
}
/*!
 * \brief read video data and tell gui to draw it
 */
void MainClient::readDataVideo()
{
    //qDebug("MainClient::readDataVideo");
    QDataStream ioStream(skt_video);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeVideo == 0){
        if(skt_video->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> sizeVideo;
    }
    if(skt_video->bytesAvailable() < (sizeVideo-sizeof(quint64)))
        return;

    if( sizeVideo != RES_KINECT_3D_W*RES_KINECT_3D_H*3 ){//si se pasa o no llega lo arreglamos
        qDebug("MainClient::readDataVideo Error tamaño datos");
        qDebug("Llegan %llu y deben ser %u bytes",sizeVideo,RES_KINECT_3D_W*RES_KINECT_3D_H*3);
        QByteArray trash;
        trash = skt_video->readAll();
        sizeVideo = 0;//to allow get next point cloud size3D
        requestNext(skt_video);
        return;
    }

    for(uint i=0;i<sizeVideo;i++){
        ioStream >> videoBuf[i];
    }
    emit printVideo();//tell GUI you've got new image frame so show

    sizeVideo = 0;
    requestNext(skt_video);
    setTime(e_video, t_video.restart());
}
/*!
 * \brief tell GUI connection error and close Video socket
 * \param socketError
 */
void MainClient::socketErrorVideo(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketErrorVideo");
    sendMessage("Error de conexión socket Video");
    finalizeVideo();
    emit socketErrorSignalVideo(false);
}

/*!
 * \brief start/stop depth connection to server
 *
 * clicked twice stops depth
 */
void MainClient::initDepth()
{
    //qDebug("MainClient::initDepth");
    if( connectedDepth ){
        finalizeDepth();
        sendMessage("- depth closed ");
        return;
    }else{
        skt_depth->connectToHost(hostAddr,DEPTHPORT);
        if( !skt_depth->waitForConnected(3000) ){
            sendMessage(skt_depth->errorString());
            qDebug("  client DEPTH NOT connected = %d",connectedDepth);
            return;
        }
        connectedDepth = 1;
        sendMessage("- depth connected");
    }
    sizeDepth = 0;
    requestNext(skt_depth);
}
/*!
 * \brief stop depth connection to server
 */
void MainClient::finalizeDepth()
{
    //qDebug("MainClient::finalizeDepth");
    if(skt_depth->state()){
        requestStop(skt_depth);
        skt_depth->disconnectFromHost();
    }
    connectedDepth = 0;
}
/*!
 * \brief read depth data and tell gui to draw it
 */
void MainClient::readDataDepth()
{
   // qDebug("MainClient::readDataDepth");
    QDataStream ioStream(skt_depth);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeDepth == 0){
        if(skt_depth->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> sizeDepth;
    }
    if(skt_depth->bytesAvailable() < sizeDepth)
        return;

    if( sizeDepth != RES_KINECT_3D_W*RES_KINECT_3D_H*2 ){
        qDebug("MainClient::readDataDepth Error tamaño datos");
        qDebug("Llegan %llu y deben ser %u bytes",sizeDepth,RES_KINECT_3D_W*RES_KINECT_3D_H*3);
        QByteArray trash;
        trash = skt_depth->readAll();
        sizeDepth = 0;
        requestNext(skt_depth);
        return;
    }

    //read depth
    for(int i=0;i<sizeDepth/2;i++){
        ioStream >> depthBuf[i];
    }
    emit printDepth();
    sizeDepth = 0;
    requestNext(skt_depth);
}
/*!
 * \brief tell GUI connection error and close Depth socket
 * \param socketError
 */
void MainClient::socketErrorDepth(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketErrorDepth");
    sendMessage("Error de conexión socket Depth");
    finalizeDepth();
    emit socketErrorSignalDepth(false);
}

/*!
 * \brief start/stop 3D connection to server
 *
 * clicked twice stops 3d
 */
void MainClient::init3D()
{
    //qDebug("MainClient::init3D");
    if( connected3D ){
        finalize3D();
        sendMessage("- 3D closed ");
        return;
    }else{
        skt_3D->connectToHost(hostAddr,THREEPORT);
        if( !skt_3D->waitForConnected(3000) ){
            sendMessage(skt_3D->errorString());
            qDebug("  client 3D NOT connected = %d",connected3D);
            return;
        }
        connected3D = 1;
        sendMessage("- 3D connected");
    }

    size3D = 0;
    requestNext(skt_3D);
}
/*!
 * \brief disconnect skt_3D from server
 */
void MainClient::finalize3D()
{
    //qDebug("MainClient::finalize3D");
    if(skt_3D->state()){
        requestStop(skt_3D);
        skt_3D->disconnectFromHost();
    }
    connected3D = 0;
}
/*!
 * \brief read 3d data and tell gui to draw it
 */
void MainClient::readData3D()
{
    //qDebug("MainClient::readData3D");
    QDataStream ioStream(skt_3D);
    ioStream.setVersion(QDataStream::Qt_5_0);
    p3Buf.resize(0);//still has memory allocated (reserved), but size = 0
    point3rgb aux3;

    if(size3D == 0){
        if(skt_3D->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> size3D;
    }
    if(skt_3D->bytesAvailable() < size3D)
        return;

    if( size3D > RES_KINECT_3D_W*RES_KINECT_3D_H*9 ){
        qDebug("MainClient::readData3D Error tamaño datos");
        qDebug("Llegan %llu y MAXIMO: %u",size3D,RES_KINECT_3D_W*RES_KINECT_3D_H*9);
        QByteArray trash;
        trash = skt_3D->readAll();
        size3D = 0;
        requestNext(skt_3D);
        return;
    }

    //-----------------------------------read
    for(uint i=0;i<(size3D/9);i++){//¿sizeof(point3rgb)=10  2+2+2+1+1+1=9? -> pragma structs
        ioStream >> aux3.x;
        ioStream >> aux3.y;
        ioStream >> aux3.z;
        ioStream >> aux3.color.rgbRed;
        ioStream >> aux3.color.rgbGreen;
        ioStream >> aux3.color.rgbBlue;
        p3Buf.push_back(aux3);
    }
    emit print3D();//signal to GUi new point cloud it's ready
    size3D = 0;
    requestNext(skt_3D);
}
/*!
 * \brief tell GUI connection error and close 3D socket
 * \param socketError
 */
void MainClient::socketError3D(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketError3D");
    sendMessage("Error de conexión socket 3D");
    finalize3D();
    emit socketErrorSignal3D(false);
}

/*!
 * \brief start/stop 2D connection to server
 *
 * clicked twice stops 2d
 */
void MainClient::init2D()
{
    //qDebug("MainClient::init2D");
    if( connected2D ){
        finalize2D();
        sendMessage("- 2D closed ");
        return;
    }else{
        skt_2D->connectToHost(hostAddr,TWOPORT);
        if( !skt_2D->waitForConnected(3000) ){
            sendMessage(skt_2D->errorString());
            qDebug("  client 2D NOT connected = %d",connected2D);
            return;
        }

        connected2D = 1;
        sendMessage("- 2D connected");
    }

    size2D = 0;
    requestNext(skt_2D);
}
/*!
 * \brief disconnect skt_2D from server
 */
void MainClient::finalize2D()
{
    //qDebug("MainClient::finalize2D");
    if(skt_2D->state()){
        requestStop(skt_2D);
        skt_2D->disconnectFromHost();
    }
    connected2D = 0;
}
/*!
 * \brief read 2d data and tell gui to draw it
 */
void MainClient::readData2D()
{
    //qDebug("MainClient::readData2D");
    QDataStream ioStream(skt_2D);
    ioStream.setVersion(QDataStream::Qt_5_0);
    p2Buf.resize(0);//still has memory allocated (reserved), but size = 0
    point2 aux2;

    if(size2D == 0){
        if(skt_2D->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> size2D;
    }
    if(skt_2D->bytesAvailable() < size2D)
        return;


    if( size2D > RES_KINECT_3D_W*RES_KINECT_3D_H*sizeof(point2) ){
        qDebug("MainClient::readData2D Error tamaño datos");
        qDebug("Llegan %llu y MAXIMO: %u",size2D,RES_KINECT_3D_W*RES_KINECT_3D_H*sizeof(point2));
        QByteArray trash;
        trash = skt_2D->readAll();
        size2D = 0;
        requestNext(skt_2D);
        return;
    }

    //---------------------------read 2D
    for(int i=0;i<(size2D/sizeof(point2));i++){
        ioStream >> aux2.x;
        ioStream >> aux2.z;
        p2Buf.push_back(aux2);
    }
    emit print2D();

    size2D = 0;
    requestNext(skt_2D);
}
/*!
 * \brief tell GUI connection error and close 2D socket
 * \param socketError
 */
void MainClient::socketError2D(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketError2D");
    sendMessage("Error de conexión socket 2D");
    finalize2D();
    emit socketErrorSignal2D(false);
}

/*!
 * \brief start/stop barrido connection to server
 *
 * clicked twice stops barrido
 */
void MainClient::initBarrido()
{
    //qDebug("MainClient::initBarrido");
    if( connectedBarrido ){
        finalizeBarrido();
        sendMessage("- barrido closed ");
        return;
    }else{
        skt_barrido->connectToHost(hostAddr,BARRIDOPORT);
        if( !skt_barrido->waitForConnected(3000) ){
            sendMessage(skt_barrido->errorString());
            qDebug("  client Barrido NOT connected = %d",connectedBarrido);
            return;
        }
        connectedBarrido = 1;
        sendMessage("- barrido connected");
    }

    sizeBarrido = 0;
    requestNext(skt_barrido);
}
/*!
 * \brief disconnect skt_barrido from server
 */
void MainClient::finalizeBarrido()
{
    //qDebug("MainClient::finalizeBarrido");
    if(skt_barrido->state()){
        requestStop(skt_barrido);
        skt_barrido->disconnectFromHost();
    }
    connectedBarrido = 0;
}
/*!
 * \brief read barrido data and tell gui to draw it
 */
void MainClient::readDataBarrido()
{
    //qDebug("MainClient::readDataBarrido");
    QDataStream ioStream(skt_barrido);
    ioStream.setVersion(QDataStream::Qt_5_0);///---------DEBUG

    if(sizeBarrido == 0){
        if(skt_barrido->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> sizeBarrido;
    }
    if(skt_barrido->bytesAvailable() < sizeBarrido)
        return;

    if( sizeBarrido != TAM_BARRIDO*sizeof(barridoBuf[0]) ){
        qDebug("MainClient::readDataBarrido Error tamaño datos");
        qDebug("Llegan %llu de %u",sizeBarrido,TAM_BARRIDO*sizeof(barridoBuf[0]));
        QByteArray trash;
        trash = skt_barrido->readAll();
        sizeBarrido = 0;
        requestNext(skt_barrido);
        return;
    }

    for(int i=0;i<TAM_BARRIDO;i++){
        ioStream >> barridoBuf[i];
    }
    emit printBarrido();

    sizeBarrido = 0;
    requestNext(skt_barrido);
}
/*!
 * \brief tell GUI connection error and close Barrido socket
 * \param socketError
 */
void MainClient::socketErrorBarrido(QAbstractSocket::SocketError socketError)
{
    //qDebug("MainClient::socketErrorBarrido");
    sendMessage("Error de conexión socket Barrido");
    finalizeBarrido();
    emit socketErrorSignalBarrido(false);
}

/*!
 * \brief start/stop accel connection to server
 *
 * clicked twice stops accel
 */
void MainClient::initAccel()
{
    //qDebug("MainClient::initAccel");
    if( connectedAccel ){
        finalizeAccel();
        sendMessage("- accel closed ");
        return;
    }else{
        skt_accel->connectToHost(hostAddr,ACCELPORT);
        if( !skt_accel->waitForConnected(3000) ){
            sendMessage(skt_accel->errorString());
            qDebug("  client Accel NOT connected = %d",connectedAccel);
            return;
        }
        connectedAccel = 1;
        sendMessage("- accel connected");
    }
    sizeAccel = 0;
    requestNext(skt_accel);
}
/*!
 * \brief disconnect skt_accel from server
 */
void MainClient::finalizeAccel()
{
    //qDebug("MainClient::finalizeAccel");
    if(skt_accel->state()){
        requestStop(skt_accel);
        skt_accel->disconnectFromHost();
    }
    connectedAccel = 0;
}
/*!
 * \brief read accel data and tell gui to draw it
 */
void MainClient::readDataAccel()
{
    //qDebug("MainClient::readDataAccel");
    QDataStream ioStream(skt_accel);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeAccel == 0){
        if(skt_accel->bytesAvailable() < (int)sizeof(quint64))
            return;
        ioStream >> sizeAccel;
    }
    if(skt_accel->bytesAvailable() < sizeAccel)
        return;

    if(sizeAccel != sizeof(acceleration.accel_x)*3){
        qDebug("MainClient::readDataAccel Error tamaño datos");
        qDebug("Llegan %llu y deben ser: %u",sizeAccel,sizeof(acceleration.accel_x)*3);
        QByteArray trash;
        trash = skt_accel->readAll();
        sizeAccel = 0;
        requestNext(skt_accel);
        return;
    }

    ioStream >> acceleration.accel_x;
    ioStream >> acceleration.accel_y;
    ioStream >> acceleration.accel_z;

    sizeAccel = 0;
    emit printTimeVector();
    //showAccel(acceleration);
    requestNext(skt_accel);//actually click and will return accell
}
/*!
 * \brief tell GUI connection error and close Accel socket
 * \param socketError
 */
void MainClient::socketErrorAccel(QAbstractSocket::SocketError socketError)
{
    qDebug("MainClient::socketErrorAccel");
    sendMessage("Error de conexión socket Accel");
    finalizeAccel();
    emit socketErrorSignalAccel(false);
}
/*!
 * \brief MainClient::showAccel
 * \param[in] a
 */
void MainClient::showAccel(accel a)
{
    //qDebug("MainClient::showAccel()");
    QString str = "aX = ";
    QString aux;
    aux.setNum(acceleration.accel_x);
    str.append(aux);
    str.append("\n");
    aux = "aY = ";
    str.append(aux);
    aux.setNum(acceleration.accel_y);
    str.append(aux);
    str.append("\n");
    aux = "aZ = ";
    str.append(aux);
    aux.setNum(acceleration.accel_z);
    str.append(aux);
    str.append("\n");
    sendMessage(str);
}

/*!
 * \brief utility function to reserve memory, init members...
 */
void MainClient::initMainClient()
{
    config = new ConfigData(this);
    connectedServer = 0;
    sizeConfig = 0;
    skt_config = new QTcpSocket(this);
    //video
    skt_video = new QTcpSocket(this);
    connectedVideo = 0;
    sizeVideo = 0;
    videoBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H*3);
    //depth
    skt_depth = new QTcpSocket(this);
    connectedDepth = 0;
    sizeDepth = 0;
    depthBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    //3D
    skt_3D = new QTcpSocket(this);
    connected3D = 0;
    size3D = 0;
    p3Buf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p3Buf.resize(0);
    //2D
    skt_2D = new QTcpSocket(this);
    connected2D = 0;
    size2D = 0;
    p2Buf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p2Buf.resize(0);
    //barrido
    skt_barrido = new QTcpSocket(this);
    connectedBarrido = 0;
    sizeBarrido = 0;
    barridoBuf.resize(TAM_BARRIDO);
    //accel
    skt_accel = new QTcpSocket(this);
    connectedAccel = 0;
    sizeAccel = 0;
    acceleration.accel_x = acceleration.accel_y = acceleration.accel_z = 0.0;
    timeVector.reserve(e_xtra);
    for(int i=0; i<e_xtra; i++) timeVector[i]=0;
}
/*!
 * \brief utility function to init connects(signals->slots)
 */
void MainClient::initConnects()
{
    connect(skt_config,SIGNAL(readyRead()),this,SLOT(readSrvKdata()));
    connect(skt_config,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketErrorSrvKinect(QAbstractSocket::SocketError)));
    connect(skt_video,SIGNAL(readyRead()),this,SLOT(readDataVideo()));
    connect(skt_video,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketErrorVideo(QAbstractSocket::SocketError)));
    connect(skt_depth,SIGNAL(readyRead()),this,SLOT(readDataDepth()));
    connect(skt_depth,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketErrorDepth(QAbstractSocket::SocketError)));
    connect(skt_3D,SIGNAL(readyRead()),this,SLOT(readData3D()));
    connect(skt_3D,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError3D(QAbstractSocket::SocketError)));
    connect(skt_2D,SIGNAL(readyRead()),this,SLOT(readData2D()));
    connect(skt_2D,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketError2D(QAbstractSocket::SocketError)));
    connect(skt_barrido,SIGNAL(readyRead()),this,SLOT(readDataBarrido()));
    connect(skt_barrido,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketErrorBarrido(QAbstractSocket::SocketError)));
    connect(skt_accel,SIGNAL(readyRead()),this,SLOT(readDataAccel()));
    connect(skt_accel,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(socketErrorAccel(QAbstractSocket::SocketError)));
}

/*!
 * \brief aux function to debug ConfigData transmission
 * \param[in] srvK
 */
void MainClient::showK(srvKinect srvK)
{
    qDebug("MainClient::showK()");
    qDebug("srvK.m_fAngulo %f",srvK.m_fAngulo);
    qDebug("srvK.m_iAnguloKinect %d",srvK.m_iAnguloKinect);
    qDebug("srvK.m_fAltura %f",srvK.m_fAltura);
    qDebug("srvK.m_fYMin %f",srvK.m_fYMin);
    qDebug("srvK.m_fYMax %f",srvK.m_fYMax);
    qDebug("srvK.m_fZMax %f",srvK.m_fZMax);
    qDebug("srvK.m_ulRefresco3D %d",srvK.m_ulRefresco3D);
    qDebug("srvK.m_usModulo3D %d",srvK.m_usModulo3D);
    qDebug("srvK.m_bEnvio3D %d", srvK.m_bEnvio3D);
    qDebug("srvK.m_bEnvio2D %d", srvK.m_bEnvio2D);
    qDebug("srvK.m_bEnvioBarrido %d", srvK.m_bEnvioBarrido);
    qDebug("srvK.m_bCompress3D %d", srvK.m_bCompress3D);
    qDebug("srvK.m_iBarridoEcu %d", srvK.m_iBarridoEcu);
    qDebug("srvK.m_iBarridoYMin %d", srvK.m_iBarridoYMin);
    qDebug("srvK.m_iBarridoYMax %d", srvK.m_iBarridoYMax);
    qDebug("srvK.m_ulRefrescoDepth %d", srvK.m_ulRefrescoDepth);
    qDebug("srvK.m_bEnvioDepth %d", srvK.m_bEnvioDepth);
    qDebug("srvK.m_bCompressDepth %d", srvK.m_bCompressDepth);
    qDebug("srvK.m_ulRefrescoColor %d", srvK.m_ulRefrescoColor);
    qDebug("srvK.m_bEnvioColor %d", srvK.m_bEnvioColor);
    qDebug("srvK.m_bCompressColor %d", srvK.m_bCompressColor);
}



