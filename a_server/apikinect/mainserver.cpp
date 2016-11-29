/*
 * Copyright (c) 2016  Gabriel Alzamora
 * Copyright (c) 2015 The Qt Company Ltd
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */


#include "mainserver.h"
#include <QNetworkInterface>
#include <QTcpServer>
#include <QTimer>
#include "apikinect.h"
#include "attendclient.h"
#include "configdata.h"

/*!
 * \class MainServer
 * \brief holds all server functionality.
 *
 * Through members that are type Apikinect, AttendClient,
 * ConfigData (config),FrameGL (ui->glWidget),
 * QTcpServer and Freenect::Freenect manage kinect and client
 * relations.
 * Apikinect allow to handle and comunicate with kinect.
 * ConfigData (config) holds configuration to handle data.
 * FrameGL (ui->glWidget) all related to 3D view.
 * QTcpServer is the server who listen to incoming clients.
 * Freenect::Freenect will hold usb context on kinect communication.
 * Apikinect hold single kinect (device) comunication.
 * AttendClient is generated to attend single client communication.
 */

/*!
 * \brief constructor
 * \param parent
 */
MainServer::MainServer(bool sirvo, QObject *parent) : QObject(parent)
{
    //qDebug("MainServer::MainServer");
    init();
    if(sirvo) startServer();
}
/*!
 * \brief destructor
 */
MainServer::~MainServer()
{
    qDebug("MainServer::~MainServer()");
    if(attendant != NULL ){
        attendant->~AttendClient();
    }
    if(getDeviceStatus()){
        stopK(currentKIndex);
    }
}

/*!
 * \brief create kinect handler.
 *
 * init 'Apikinect device' to handle kinect of indexK.
 * \param [in] indexK.
 */
void MainServer::startK(int indexK)
{
    //qDebug("MainServer::startK");
    device = &freenect.createDevice<Apikinect>(indexK);
    device->startVideo();
    device->startDepth();
    int i(depthVoid());
    while(depthVoid()){
        device->stopDepth();
        device->startDepth();
        device->stopVideo();
        device->startVideo();
        qDebug("MainServer::startK  reiniciamos device");
        i++;
        if(i>5){
            qDebug("MainServer::startK  problemas con sensor: comprueba conexiones y reinicia.");
            stopK(indexK);
            return;
        }
    }
    currentKIndex = indexK;
}
/*!
 * \brief destroy kinect handler.
 * \param [in] index kinect handler index to be destroyed.
 */
void MainServer::stopK(int indexK)
{
    device->stopDepth();
    device->stopVideo();
    freenect.deleteDevice(indexK);
    currentKIndex = -1;
    device = NULL;
}
/*!
 * \brief set current led option and kinect angle on active kinect.
 *
 * When any data on 'ConfigData' object change it sends the order to
 * update to active kinect
 */
void MainServer::updateKinect()
{
    if( device != NULL ){
        device->setLed(freenect_led_options(config->getLedOption()));
        device->setTiltDegrees(double(config->srvK.m_iAnguloKinect));
    }
}
/*!
 * \brief return computer ip as QString
 * \return QString with computer ip
 */
QString MainServer::getServerIp()
{
    QString ip;
    QList<QHostAddress> ipList = QNetworkInterface::allAddresses();
    // use first non-localhost IPv4 address
    for (int i = 0; i < ipList.size(); ++i) {
        if( (ipList.at(i) != QHostAddress::LocalHost) && ipList.at(i).toIPv4Address() ) {
            ip = ipList.at(i).toString();
            break;
        }
    }
    // if none, then use localhost
    if (ip.isEmpty()) ip = QHostAddress(QHostAddress::LocalHost).toString();
    return ip;
}
/*!
 * \brief tells you if device pointer is not null (m_kinect.start() launched)
 * \return int = 0 if device not started i.e. *device == NULL
 */
int MainServer::getDeviceStatus()
{
    if( this->device == NULL ){
        return 0;
    }
    return 1;
}
/*!
 * \brief set srvKinect data sended by client
 * \param [in] newSrvK
 *
 * set client current srvKinect to MainServer::ConfigData::srvK
 */
void MainServer::setSrvKinect(srvKinect newSrvK)
{
    //qDebug("MainServer::setSrvKinect");
    //adaptamos QTimers si varia Refresco
    if ( !newSrvK.m_bEnvioColor && timerVideo->isActive() ){
        timerVideo->stop();
    }else if( config->srvK.m_ulRefrescoColor != newSrvK.m_ulRefrescoColor ){
        timerVideo->start(newSrvK.m_ulRefrescoColor);
    }
    if ( !newSrvK.m_bEnvioDepth && timerDepth->isActive() ){
        timerDepth->stop();
    }else if( config->srvK.m_ulRefrescoDepth != newSrvK.m_ulRefrescoDepth ){
        timerDepth->start(newSrvK.m_ulRefrescoDepth);
    }
    if ( !(config->srvK.m_bEnvio3D || config->srvK.m_bEnvio2D || config->srvK.m_bEnvioBarrido) && timer3D->isActive() ){
        timer3D->stop();
    }else if( config->srvK.m_ulRefresco3D != newSrvK.m_ulRefresco3D ){
        timer3D->start(newSrvK.m_ulRefresco3D);
    }
    //actualizamos ángulo en kinect
    if( config->srvK.m_iAnguloKinect != newSrvK.m_iAnguloKinect ){
        config->setSrvK(newSrvK);
        emit updateSrvKinect(config->srvK);
        updateKinect();
        return;
    }
    config->setSrvK(newSrvK);
    emit updateSrvKinect(config->srvK);
}
/*!
 * \brief set srvKinect data sended by GUI
 * \param [in] newSrvK
 *
 * set client current srvKinect to MainServer::ConfigData::srvK
 */
void MainServer::setGUISrvKinect(srvKinect newSrvK)
{
    //qDebug("MainServer::setGUISrvKinect");
    //adaptamos QTimers si varia Refresco
    if ( !newSrvK.m_bEnvioColor && timerVideo->isActive() ){
        timerVideo->stop();
    }else if( config->srvK.m_ulRefrescoColor != newSrvK.m_ulRefrescoColor ){
        timerVideo->start(newSrvK.m_ulRefrescoColor);
    }
    if ( !newSrvK.m_bEnvioDepth && timerDepth->isActive() ){
        timerDepth->stop();
    }else if( config->srvK.m_ulRefrescoDepth != newSrvK.m_ulRefrescoDepth ){
        timerDepth->start(newSrvK.m_ulRefrescoDepth);
    }
    if ( !(config->srvK.m_bEnvio3D || config->srvK.m_bEnvio2D || config->srvK.m_bEnvioBarrido) && timer3D->isActive() ){
        timer3D->stop();
    }else if( config->srvK.m_ulRefresco3D != newSrvK.m_ulRefresco3D ){
        timer3D->start(newSrvK.m_ulRefresco3D);
    }
    //actualizamos ángulo en kinect
    if( config->srvK.m_iAnguloKinect != newSrvK.m_iAnguloKinect ){
        config->setSrvK(newSrvK);
        emit updateSrvKinect(config->srvK);
        updateKinect();
        return;
    }
    config->setSrvK(newSrvK);
    emit updateClientSrvKinect(newSrvK);
}

srvKinect MainServer::getSrvKinect()
{
    return config->srvK;
}

/*!
 * \brief getter for currentKIndex
 * \return currentKIndex
 */
int MainServer::getCurrentKIndex()
{
    return currentKIndex;
}
/*!
 * \brief setter of currentKIndex
 * \param index
 */
void MainServer::setCurrentKIndex(int index)
{
    currentKIndex = index;
}
/*!
 * \brief get number of kinect detected
 * \return number of kinect
 */
int MainServer::getKnumber()
{
    return freenect.deviceCount();
}
/*!
 * \brief getter for timeVector, return requested time
 * \param opt wich time will return
 * \return requested time in ms
 */
int MainServer::getTime(eOption opt)
{
    return timeVector[opt];
}
/*!
 * \brief getter for acceleration
 * \return
 */
accel MainServer::getAccel()
{
    return a;
}

/*!
 * \brief MainServer::go
 */
void MainServer::go()
{
    if ( config->srvK.m_bEnvioColor ){
        nextVideoFrame();
        timerVideo->start(config->srvK.m_ulRefrescoColor);
    }
    if ( config->srvK.m_bEnvioDepth ){
        nextDepthFrame();
        timerDepth->start(config->srvK.m_ulRefrescoDepth);
    }
    if ( config->srvK.m_bEnvio3D || config->srvK.m_bEnvio2D || config->srvK.m_bEnvioBarrido ){
        next3DFrame();
        timer3D->start(config->srvK.m_ulRefresco3D);
    }
}
/*!
 * \brief MainServer::stop
 */
void MainServer::stop()
{
    if (timerVideo->isActive()) timerVideo->stop();
    if (timerDepth->isActive()) timerDepth->stop();
    if (timer3D->isActive()) timer3D->stop();
    if (currentKIndex != -1) stopK(currentKIndex);
}

/*!
 * \brief convenience function to initiate members
 */
void MainServer::init()
{
    //apikinect
    device = NULL;
    numKinects = getKnumber();
    currentKIndex = -1;
    flag = false;
    //config data
    config = new ConfigData(this);

    //buffers
    videoBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H*3);
    depthBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p3rgbBuf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);//max number of points
    p3rgbBuf.resize(0);//initially we have none
    p2Buf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p2Buf.resize(0);
    barridoBuf.resize(TAM_BARRIDO);
    a.accel_x = a.accel_y = a.accel_z = 0;
    timeVector.resize(e_xtra);
    //pBuff
    structBuffers.ptrVideoBuf = &videoBuf;
    structBuffers.ptrDepthBuf = &depthBuf;
    structBuffers.ptrP3Buf = &p3rgbBuf;
    structBuffers.ptrP2Buf = &p2Buf;
    structBuffers.ptrBarridoBuf = &barridoBuf;
    structBuffers.ptrAccel = &a;
    //server
    server = new QTcpServer(this);
    //timers
    timer3D = new QTimer(this);
    timerDepth = new QTimer(this);
    timerVideo = new QTimer(this);
    connect(timer3D,SIGNAL(timeout()),this,SLOT(next3DFrame()));
    connect(timerDepth,SIGNAL(timeout()),this,SLOT(nextDepthFrame()));
    connect(timerVideo,SIGNAL(timeout()),this,SLOT(nextVideoFrame()));
}
/*!
 * \brief function to asure that depth data are available
 * \return 1 if no depth data, otherwise return 0
 */
int MainServer::depthVoid()
{
    //qDebug("MainServer::depthVoid");
    if(getDeviceStatus()){
        device->getDepth(depthBuf);
        for(int i = 0; i < RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H;i++){
            if( depthBuf[i] > 0 ){
                return 0;
            }
        }
    }else{
        qDebug("MainServer::depthVoid  Intentas usar un device == NULL");
        return 0;
    }
    return 1;
}

/*!
 * \brief start QTcpServer listening at port 9999 and connect to attendNewClient()
 */
void MainServer::startServer()
{
    qDebug("MainServer::startServer");
    if( !server->listen(QHostAddress::Any,SRVKPORT) ){
        qDebug("  server do not listen, closed\ntry to restart.");
        server->close();
        return;
    }
    connect(server, SIGNAL(newConnection()), this, SLOT(attendNewClient()));
}
/*!
 * \brief when client connection incoming create a new 'AttendClient' and bind
 */
void MainServer::attendNewClient()
{
    //qDebug("MainServer::attendNewClient");
    attendant = new AttendClient(server->nextPendingConnection(),&structBuffers,this);
    if( attendant == NULL ) qDebug("BAD_ALLOC  AttendClient");
    //attendVector.push_back(attendant);

    connect(attendant,SIGNAL(newSrvKinect(srvKinect)),this,SLOT(setSrvKinect(srvKinect)));
    connect(this,SIGNAL(updateClientSrvKinect(srvKinect)),attendant,SLOT(sendSrvKinect(srvKinect)));
}

/*!
 * \brief MainServer::nextVideoFrame
 */
void MainServer::nextVideoFrame()
{
    device->getRGB(videoBuf);
    emit printVideo();
}
/*!
 * \brief MainServer::nextDepthFrame
 */
void MainServer::nextDepthFrame()
{
    device->getDepth(depthBuf);
    emit printDepth();
}
/*!
 * \brief calculate 3D points in different formats
 *
 * Calculate from video + depth what is going to be sended
 * to client so you cannot show what you don't serve
 */
void MainServer::next3DFrame()
{
    if( config->srvK.m_bEnvio3D && config->srvK.m_bEnvio2D && config->srvK.m_bEnvioBarrido ){
        ///------------------------------------------------------------------TIEMPOS, parte se calcula aquí
        ///------------------------------------------------------------------
        device->getAll( &structBuffers, &config->srvK );
        emit print3D();
        emit print2D();
        emit printBarrido();
    }else if (config->srvK.m_bEnvio3D && config->srvK.m_bEnvio2D) {
        device->get2and3( &structBuffers, &config->srvK );
        emit print3D();
        emit print2D();
    }else if (config->srvK.m_bEnvio3D && config->srvK.m_bEnvioBarrido) {
        device->get3dBarrido( &structBuffers, &config->srvK );
        emit print3D();
        emit printBarrido();
    }else if (config->srvK.m_bEnvio2D && config->srvK.m_bEnvioBarrido) {
        device->get2dBarrido( &structBuffers, &config->srvK );
        emit print2D();
        emit printBarrido();
    }else if(config->srvK.m_bEnvio3D){
        device->get3d(&structBuffers, &config->srvK );
        emit print3D();
    }else if(config->srvK.m_bEnvio2D){
        device->get2(&structBuffers, &config->srvK );
        emit print2D();
    }else if(config->srvK.m_bEnvioBarrido){
        device->getBarrido(&structBuffers, &config->srvK );
        emit printBarrido();
    }
}
