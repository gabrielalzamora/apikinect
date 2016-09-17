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
#include "maincore.h"
#include "apikinect.h"

/*!
 * \class MainCore
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
MainCore::MainCore(QObject *parent) : QObject(parent)
{
    //qDebug("MainCore::MainCore");
    init();
    //startServer();
}
/*!
 * \brief destructor
 */
MainCore::~MainCore()
{
    qDebug("MainCore::~MainCore()");
    if(currentKIndex !=-1){

        stopK(currentKIndex);
    }
}

/*!
 * \brief create kinect handler.
 *
 * init 'Apikinect device' to handle kinect of indexK.
 * \param [in] indexK.
 */
void MainCore::startK(int indexK)
{
    device = &freenect.createDevice<Apikinect>(indexK);
    device->startVideo();
    device->startDepth();
    currentKIndex = indexK;
}
/*!
 * \brief destroy kinect handler.
 * \param [in] index kinect handler index to be destroyed.
 */
void MainCore::stopK(int indexK)
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
void MainCore::updateKinect()
{
    if( device != NULL ){
        device->setLed(freenect_led_options(config->getLedOption()));
        device->setTiltDegrees(double(config->getSrvK().m_iAnguloKinect));
    }
}
/*!
 * \brief set srvKinect data sended by client
 * \param [in] sk
 * client current srvKinect
 */
void MainCore::updateSrvKinect(srvKinect newSrvK)
{
//    qDebug("MainCore::updateKinect");
    config->setSrvK(newSrvK);
}

/*!
 * \brief getter for currentKIndex
 * \return currentKIndex
 */
int MainCore::getCurrentKIndex()
{
    return currentKIndex;
}
/*!
 * \brief setter of currentKIndex
 * \param index
 */
void MainCore::setCurrentKIndex(int index)
{
    currentKIndex = index;
}
/*!
 * \brief get number of kinect detected
 * \return number of kinect
 */
int MainCore::getKnumber()
{
    return freenect.deviceCount();
}
/*!
 * \brief getter for timeVector, return requested time
 * \param opt wich time will return
 * \return requested time in ms
 */
int MainCore::getTime(eOption opt)
{
    return timeVector[opt];
}
/*!
 * \brief getter for acceleration
 * \return
 */
accel MainCore::getAccel()
{
    return a;
}


/*!
 * \brief convenience function to initiate members
 */
void MainCore::init()
{
    //apikinect
    device = NULL;
    numKinects = getKnumber();
    currentKIndex = -1;
    flag = false;
    //buffers
    videoBuf.resize(640*480*3);
    depthBuf.resize(640*480);
    p3rgbBuf.reserve(300000);//max number of points
    p3rgbBuf.resize(0);//initially we have none
    p2Buf.reserve(300000);
    p2Buf.resize(0);
    barridoBuf.resize(360);
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
}


/*!
 * \brief start QTcpServer listening at port 9999 and connect to attendNewClient()
 */
void MainCore::startServer()
{
    qDebug("MainCore::startServer");
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
void MainCore::attendNewClient()///------test with concurrent clients----------DEBUG
{
    qDebug("MainCore::attendNewClient");
    attendant = new AttendClient(server->nextPendingConnection(),&structBuffers,this);
    if( attendant == NULL ) qDebug("BAD_ALLOC  AttendClient");
    //attendVector.push_back(attendant);

    connect(attendant,SIGNAL(newSrvKinect(srvKinect)),this,SLOT(updateSrvKinect(srvKinect)));
}
