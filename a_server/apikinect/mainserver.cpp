/*
 * Copyright (c) 2016  Gabriel Alzamora
 * Copyright (c) 2015 The Qt Company Ltd
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <QNetworkInterface>
#include <QTcpServer>
#include <QTimer>
#include "mainserver.h"
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
    server->deleteLater();
    timer3D->deleteLater();
    timerDepth->deleteLater();
    timerVideo->deleteLater();
    timerTime->deleteLater();
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
 * \brief destroy indexK kinect handler.
 * \param [in] index of kinect whose handler will be destroyed.
 */
void MainServer::stopK(int indexK)
{
    //qDebug("MainServer::stopK");
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
    //qDebug("MainServer::updateKinect");
    if( device != NULL ){
        device->setLed(freenect_led_options(config->getLedOption()));
        device->setTiltDegrees(double(config->getSrvK().m_iAnguloKinect));
    }
}
/*!
 * \brief return computer ip as QString
 * \return QString with computer ip
 */
QString MainServer::getServerIp()
{
    //qDebug("MainServer::getServerIp");
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
    //qDebug("MainServer::getDeviceStatus");
    if( this->device == NULL ){
        return 0;
    }
    return 1;
}
/*!
 * \brief set srvKinect data in MainServer::ConfigData::srvKinect
 * \param[in] newSrvK
 *
 * set client current srvKinect to MainServer::ConfigData::srvK
 * usually as new srvKinect sended by client and signaling to
 * GUI to show updated config data.
 */
void MainServer::setSrvKinect(srvKinect newSrvK)
{
    //qDebug("MainServer::setSrvKinect");
    if(getDeviceStatus()){//adaptamos QTimers si varia Refresco
        if( !newSrvK.m_bEnvioColor && timerVideo->isActive() ){
            timerVideo->stop();
        }else if( config->getSrvK().m_ulRefrescoColor != newSrvK.m_ulRefrescoColor ){
            timerVideo->start(newSrvK.m_ulRefrescoColor);
        }
        if( !newSrvK.m_bEnvioDepth && timerDepth->isActive() ){
            timerDepth->stop();
        }else if( config->getSrvK().m_ulRefrescoDepth != newSrvK.m_ulRefrescoDepth ){
            timerDepth->start(newSrvK.m_ulRefrescoDepth);
        }
        if( !(config->getSrvK().m_bEnvio3D || config->getSrvK().m_bEnvio2D || config->getSrvK().m_bEnvioBarrido) && timer3D->isActive() ){
            timer3D->stop();
        }else if( config->getSrvK().m_ulRefresco3D != newSrvK.m_ulRefresco3D ){
            timer3D->start(newSrvK.m_ulRefresco3D);
        }
        //actualizamos ángulo en kinect
        if( config->getSrvK().m_iAnguloKinect != newSrvK.m_iAnguloKinect ){
            config->setSrvK(newSrvK);
            emit updateSrvKinect(newSrvK);
            updateKinect();
            return;
        }
    }
    config->setSrvK(newSrvK);
    emit updateSrvKinect(newSrvK);//warn to update GUI
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
    if(getDeviceStatus()){//adaptamos QTimers si varia Refresco
        if( !newSrvK.m_bEnvioColor && timerVideo->isActive() ){
            timerVideo->stop();
        }else if(newSrvK.m_bEnvioColor){
            timerVideo->start(newSrvK.m_ulRefrescoColor);
        }
        if( !newSrvK.m_bEnvioDepth && timerDepth->isActive() ){
            timerDepth->stop();
        }else if( newSrvK.m_bEnvioDepth ){
            timerDepth->start(newSrvK.m_ulRefrescoDepth);
        }
        if( !(config->getSrvK().m_bEnvio3D || config->getSrvK().m_bEnvio2D || config->getSrvK().m_bEnvioBarrido) && timer3D->isActive() ){
            timer3D->stop();
        }else if( newSrvK.m_bEnvio3D || newSrvK.m_bEnvio2D || newSrvK.m_bEnvioBarrido ){
            timer3D->start(newSrvK.m_ulRefresco3D);
        }
        //actualizamos ángulo en kinect
        if( config->getSrvK().m_iAnguloKinect != newSrvK.m_iAnguloKinect ){
            config->setSrvK(newSrvK);
            emit updateSrvKinect(config->getSrvK());
            updateKinect();
            return;
        }
    }
    config->setSrvK(newSrvK);
    emit updateClientSrvKinect(newSrvK);//warn to update client
}
/*!
 * \brief getter for current srvKinect
 * \return current srvKinect in MainServer::ConfigData::srvK
 */
srvKinect MainServer::getSrvKinect()
{
    //qDebug("MainServer::getSrvKinect");
    return config->getSrvK();
}
/*!
 * \brief setter for led option
 * \param ledOpt
 */
void MainServer::setLed(int ledOpt)
{
    //qDebug("MainServer::setLed");
    if( 0 < ledOpt || ledOpt < 7 ) config->setLedOption(ledOpt);
}
/*!
 * \brief getter of led option
 * \return
 */
int MainServer::getLed()
{
    return config->getLedOption();
}
/*!
 * \brief setter IR
 * \param irOpt active or not
 */
void MainServer::setIR(bool irOpt)
{
    config->setIrOption(irOpt);
}
/*!
 * \brief getter IR
 * \return true if enabled otherwise false
 */
bool MainServer::getIR()
{
    return config->getIrOption();
}

/*!
 * \brief getter for currentKIndex
 * \return currentKIndex
 */
int MainServer::getCurrentKIndex()
{
    //qDebug("MainServer::getCurrentKIndex");
    return currentKIndex;
}
/*!
 * \brief setter of currentKIndex
 * \param index
 */
void MainServer::setCurrentKIndex(int index)
{
    //qDebug("MainServer::setCurrentKIndex");
    currentKIndex = index;
}
/*!
 * \brief get number of kinect detected
 * \return number of kinect
 */
int MainServer::getKnumber()
{
    //qDebug("MainServer::getKnumber");
    return freenect.deviceCount();
}
/*!
 * \brief getter for timeVector, return requested time
 * \param opt wich time will return
 * \return requested time in ms
 */
int MainServer::getTime(eOption opt)
{
    //qDebug("MainServer::getTime");
    return timeVector[opt];
}
/*!
 * \brief setter of timeVector.
 * \param opt [in] eOption of timeVector to update.
 * \param msec [in] value to set on eOption.
 */
void MainServer::setTime(eOption opt, int msec)
{
    //qDebug("MainServer::setTime");
    timeVector[opt]=msec;
}
/*!
 * \brief getter for acceleration
 * \return
 */
accel MainServer::getAccel()
{
    //qDebug("MainServer::getAccel");
    return a;
}

/*!
 * \brief MainServer::go
 */
void MainServer::go()
{
    //qDebug("MainServer::go");
    if(currentKIndex<0 && numKinects){
        startK();
        currentKIndex=0;
        updateKinect();
    }else if(currentKIndex<numKinects){
        startK(currentKIndex);
        updateKinect();
        emit updateSrvKinect(config->getSrvK());
    }else{
        qDebug("MainServer::go()  intentas arrarcar Kinect con índice fuera de límites");
        return;
    }

    if ( config->getSrvK().m_bEnvioColor ){
        nextVideoFrame();
        timerVideo->start(config->getSrvK().m_ulRefrescoColor);
    }
    if ( config->getSrvK().m_bEnvioDepth ){
        nextDepthFrame();
        timerDepth->start(config->getSrvK().m_ulRefrescoDepth);
    }
    if ( config->getSrvK().m_bEnvio3D || config->getSrvK().m_bEnvio2D || config->getSrvK().m_bEnvioBarrido ){
        next3DFrame();
        timer3D->start(config->getSrvK().m_ulRefresco3D);
    }
    timerTime->start(500);
}
/*!
 * \brief MainServer::stop
 */
void MainServer::stop()
{
    //qDebug("MainServer::stop");
    if(timerVideo->isActive()) timerVideo->stop();
    if(timerDepth->isActive()) timerDepth->stop();
    if(timer3D->isActive()) timer3D->stop();
    if(timerTime->isActive()) timerTime->stop();
    if (currentKIndex != -1){
        stopK(currentKIndex);
        currentKIndex = -1;
    }
}

/*!
 * \brief convenience function to initiate members
 */
void MainServer::init()
{
    //qDebug("MainServer::init");
    //apikinect
    device = NULL;
    numKinects = getKnumber();
    currentKIndex = -1;
    //config data
    config = new ConfigData(this);


    //buffers
    videoBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H*3);
    depthBuf.resize(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p3rgbBuf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);//reserved memory
    p3rgbBuf.resize(0);//initially we have no points so size = 0
    p2Buf.reserve(RES_KINECT_VIDEO_W*RES_KINECT_VIDEO_H);
    p2Buf.resize(0);
    barridoBuf.resize(TAM_BARRIDO);
    a.accel_x = a.accel_y = a.accel_z = 0;
    timeVector.reserve(e_xtra);
    for(int i=0; i<e_xtra; i++) timeVector[i]=0;
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
    timerTime = new QTimer(this);
    connect(timer3D,SIGNAL(timeout()),this,SLOT(next3DFrame()));
    connect(timerDepth,SIGNAL(timeout()),this,SLOT(nextDepthFrame()));
    connect(timerVideo,SIGNAL(timeout()),this,SLOT(nextVideoFrame()));
    connect(timerTime,SIGNAL(timeout()),this,SLOT(nextTimeVector()));
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
    //qDebug("MainServer::startServer");
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
    int attendID(attendVector.size());
    attendant = new AttendClient(server->nextPendingConnection(),&structBuffers, attendID,this);
    if( attendant == NULL ){
        qDebug("BAD_ALLOC  AttendClient not created");
        return;
    }
    attendVector.push_back(attendant);

    //connect(attendant,SIGNAL(newSrvKinect(srvKinect)),this,SLOT(setSrvKinect(srvKinect)));
    //connect(this,SIGNAL(updateClientSrvKinect(srvKinect)),attendant,SLOT(sendSrvKinect(srvKinect)));
    connect(attendVector[attendID],SIGNAL(newSrvKinect(srvKinect)),this,SLOT(setSrvKinect(srvKinect)));
    connect(this,SIGNAL(updateClientSrvKinect(srvKinect)),attendVector[attendID],SLOT(sendSrvKinect(srvKinect)));
}

/*!
 * \brief load next Video frame to Video vector
 */
void MainServer::nextVideoFrame()
{
    //qDebug("MainServer::nextVideoFrame");
    device->getRGB(videoBuf);
    emit printVideo();
}
/*!
 * \brief load next Depth frame to depth vector
 */
void MainServer::nextDepthFrame()
{
    //qDebug("MainServer::nextDepthFrame");
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
    //qDebug("MainServer::next3DFrame");
    srvKinect k;
    config->copySrvK(&k);
    QTime t;
    t.start();
    if( k.m_bEnvio3D && k.m_bEnvio2D && k.m_bEnvioBarrido ){
        device->getAll( &structBuffers, &k );
        emit print3D();
        emit print2D();
        emit printBarrido();
        timeVector[e_3]=t.elapsed();
    }else if (config->getSrvK().m_bEnvio3D && config->getSrvK().m_bEnvio2D) {
        device->get2and3( &structBuffers, &k );
        emit print3D();
        emit print2D();
        timeVector[e_3]=t.elapsed();
    }else if (config->getSrvK().m_bEnvio3D && config->getSrvK().m_bEnvioBarrido) {
        device->get3dBarrido( &structBuffers, &k );
        emit print3D();
        emit printBarrido();
        timeVector[e_3]=t.elapsed();
    }else if (config->getSrvK().m_bEnvio2D && config->getSrvK().m_bEnvioBarrido) {
        device->get2dBarrido( &structBuffers, &k );
        emit print2D();
        emit printBarrido();
        timeVector[e_2]=t.elapsed();
    }else if(config->getSrvK().m_bEnvio3D){
        device->get3d(&structBuffers, &k );
        emit print3D();
        timeVector[e_3]=t.elapsed();
    }else if(config->getSrvK().m_bEnvio2D){
        device->get2(&structBuffers, &k );
        emit print2D();
        timeVector[e_2]=t.elapsed();
    }else if(config->getSrvK().m_bEnvioBarrido){
        device->getBarrido(&structBuffers, &k );
        emit printBarrido();
        timeVector[e_barrido]=t.elapsed();
    }
}
/*!
 * \brief lets tell GUI to show timeVector + acceleration
 */
void MainServer::nextTimeVector()
{
    //qDebug("MainServer::nextTimeVector");
    device->getAccel(a);
    emit printTimeVector();
}

/*!
 * \brief aux function to debug ConfigData transmission
 * \param[in] srvK
 */
void MainServer::showK(srvKinect srvK)
{
    qDebug("MainServert::showK()");
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
