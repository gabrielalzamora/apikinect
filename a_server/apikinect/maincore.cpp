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
    numKinects = getKnumber();

    init();
    startServer();
}
/*!
 * \brief destructor
 */
MainCore::~MainCore()
{
    qDebug("MainCore::~MainCore()");
    if(currentKIndex !=-1){
        stoploop();
        stopK(currentKIndex);
    }
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
        device->setLed(freenect_led_options(config->ledOption));
        device->setTiltDegrees(double(config->srvK.m_iAnguloKinect));
    }
}
/*!
 * \brief set srvKinect data sended by client
 * \param [in] sk
 * client current srvKinect
 */
void MainCore::updateSrvKinect(srvKinect newSrvK)
{
//    qDebug("MainServer::updateKinect");
    config->srvK.m_fAngulo = newSrvK.m_fAngulo;
    config->srvK.m_iAnguloKinect = newSrvK.m_iAnguloKinect;
    config->srvK.m_fAltura = newSrvK.m_fAltura;
    config->srvK.m_fYMin = newSrvK.m_fYMin;
    config->srvK.m_fYMax = newSrvK.m_fYMax;
    config->srvK.m_fZMax = newSrvK.m_fZMax;
    config->srvK.m_ulRefresco3D = newSrvK.m_ulRefresco3D;
    config->srvK.m_usModulo3D = newSrvK.m_usModulo3D;
    config->srvK.m_bEnvio3D = newSrvK.m_bEnvio3D;
    config->srvK.m_bEnvio2D = newSrvK.m_bEnvio2D;
    config->srvK.m_bEnvioBarrido = newSrvK.m_bEnvioBarrido;
    config->srvK.m_bCompress3D = newSrvK.m_bCompress3D;
    config->srvK.m_iBarridoEcu = newSrvK.m_iBarridoEcu;
    config->srvK.m_iBarridoYMin = newSrvK.m_iBarridoYMin;
    config->srvK.m_iBarridoYMax = newSrvK.m_iBarridoYMax;
    config->srvK.m_ulRefrescoDepth = newSrvK.m_ulRefrescoDepth;
    config->srvK.m_bEnvioDepth = newSrvK.m_bEnvioDepth;
    config->srvK.m_bCompressDepth = newSrvK.m_bCompressDepth;
    config->srvK.m_ulRefrescoColor = newSrvK.m_ulRefrescoColor;
    config->srvK.m_bEnvioColor = newSrvK.m_bEnvioColor;
    config->srvK.m_bCompressColor = newSrvK.m_bCompressColor;

    emit updateKinect();
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
 * \brief getter for currentKIndex
 * \return currentKIndex
 */
int MainCore::getCurrentKIndex()
{
    return currentKIndex;
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
    currentKIndex = -1;
    flag = false;
    videoBuf.resize(640*480*3);
    depthBuf.resize(640*480);
    p3rgbBuf.reserve(300000);//max number of points
    p3rgbBuf.resize(0);//initially we have none
    p2Buf.reserve(300000);
    p2Buf.resize(0);
    barridoBuf.resize(360);
    a.accel_x = a.accel_y = a.accel_z = 0;
    structBuffers.ptrVideoBuf = &videoBuf;
    structBuffers.ptrDepthBuf = &depthBuf;
    structBuffers.ptrP3Buf = &p3rgbBuf;
    structBuffers.ptrP2Buf = &p2Buf;
    structBuffers.ptrBarridoBuf = &barridoBuf;
    structBuffers.ptrAccel = &a;
    timeVector.resize(e_xtra);
    //server
    mainServer = new QTcpServer(this);
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
 * \brief working horse retrieving video & depth info.
 *
 * Get video and depth info to pour it into buffers as
 * p3rgbBuf (point cloud: 3D+color)...
 */
void MainCore::loop()
{
    flag = 1;
    while( flag ){

        QTime t;
        timeVector[e_video]=0;
        timeVector[e_depth]=0;
        timeVector[e_3]=0;
        timeVector[e_2]=0;
        timeVector[e_barrido]=0;
        timeVector[e_accel]=0;

        if( config->srvK.m_bEnvioColor ){
            t.start();//---------------------------------------------time.start
            device->getRGB(videoBuf);
            timeVector[e_video]=t.elapsed();//---------------------timeVector[e_video]
            if(ui->cb_video->isChecked())
                videoDataReady();//paint video on gvVideo
        }

        if( config->srvK.m_bEnvioDepth )
            t.start();//---------------------------------------------time.start
            device->getDepth(depthBuf);
            timeVector[e_depth]=t.elapsed();//---------------------timeVector[e_depth]
            if(ui->cb_depth->isChecked())
                depthDataReady();//paint depth on gvDepth

//        qApp->processEvents();//stay responsive to button click

        if( config->srvK.m_bEnvio2D && config->srvK.m_bEnvio3D && config->srvK.m_bEnvioBarrido){//all buffers
            //qDebug("  2D+3D+Barrido");
            t.start();//---------------------------------------------time.start
            device->getAll(&structBuffers,&config->srvK);//---get all points 3d, 2d & barrido
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_2->isChecked())
                ui->glWidget->setCloud(p2Buf);//send new point cloud to FrameGL
            if(ui->cb_3->isChecked())
                ui->glWidget->setCloud(p3rgbBuf);
            if(ui->cb_2->isChecked() || ui->cb_3->isChecked())
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(!(config->srvK.m_bEnvio2D) && config->srvK.m_bEnvio3D && config->srvK.m_bEnvioBarrido){
            //qDebug("  3D+Barrido");
            t.start();//---------------------------------------------time.start
            device->get3dBarrido(&structBuffers,&config->srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_3->isChecked()){
                ui->glWidget->setCloud(p3rgbBuf);//send FrameGL new point cloud
                ui->glWidget->repaint();//paint points cloud on oglwidget through FrameGL
            }
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(config->srvK.m_bEnvio2D && !(config->srvK.m_bEnvio3D) && config->srvK.m_bEnvioBarrido){
            //qDebug("  2D+Barrido");
            t.start();//---------------------------------------------time.start
            device->get2dBarrido(&structBuffers,&config->srvK);
            timeVector[e_2] = t.elapsed();//---------------------timeVector[e_2]
            if(ui->cb_2->isChecked()){
                ui->glWidget->setCloud(p2Buf);
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            }
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(config->srvK.m_bEnvio2D && config->srvK.m_bEnvio3D && !(config->srvK.m_bEnvioBarrido)){
            //qDebug("  2D+3D");
            t.start();//---------------------------------------------time.start
            device->get2and3(&structBuffers,&config->srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_2->isChecked())
                ui->glWidget->setCloud(p2Buf);//send new point cloud to FrameGL
            if(ui->cb_3->isChecked())
                ui->glWidget->setCloud(p3rgbBuf);
            if(ui->cb_2->isChecked() || ui->cb_3->isChecked())
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
        }else if(config->srvK.m_bEnvio3D){
            //qDebug("  3D");
            t.start();//---------------------------------------------time.start
            device->get3d(&structBuffers,&config->srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_3->isChecked()){
                ui->glWidget->setCloud(p3rgbBuf);//send FrameGL new point cloud
                ui->glWidget->repaint();//paint points cloud on oglwidget through FrameGL
            }
        }else if(config->srvK.m_bEnvio2D){
            //qDebug("  2D");
            t.start();//---------------------------------------------time.start
            device->get2(&structBuffers,&config->srvK);
            timeVector[e_2] = t.elapsed();//---------------------timeVector[e_2]
            if(ui->cb_2->isChecked()){
                ui->glWidget->setCloud(p2Buf);
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            }
        }else if(config->srvK.m_bEnvioBarrido){
            //qDebug("  Barrido");
            t.start();//---------------------------------------------time.start
            device->getBarrido(&structBuffers,&config->srvK);
            timeVector[e_barrido] = t.elapsed();//---------------------timeVector[e_barrido]
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }
        device->getAccel(a);
        printTimeVector(timeVector);
//------------------------------------------pinta aceleraciones---------------
        qApp->processEvents();//stay responsive to button click
    }
}
/*!
 * \brief convinience funtion to stop loop seting while(flag)=0.
 */
void MainCore::stoploop()
{
    flag = 0;
}
/*!
 * \brief aux function to control time spend in calculus or painting.
 * \param [out] timeV vector to save data.
 */
void MainCore::printTimeVector(std::vector<int> &timeV)
{
    QString str,aux;
    str = "get video = ";
    aux.setNum(timeVector[e_video]);
    str.append(aux);
    aux = " \nget depth = ";
    str.append(aux);
    aux.setNum(timeVector[e_depth]);
    str.append(aux);
    aux = " \nget 3D = ";
    str.append(aux);
    aux.setNum(timeVector[e_3]);
    str.append(aux);
    aux = "\nget 2D = ";
    str.append(aux);
    aux.setNum(timeV[e_2]);
    str.append(aux);
    aux = "\nget Barrido = ";
    str.append(aux);
    aux.setNum(timeV[e_barrido]);
    str.append(aux);
    //pinta las aceleraciones----------------------accel
    aux = "\n  accel X = ";
    str.append(aux);
    aux.setNum(a.accel_x);
    str.append(aux);
    aux = "\n  accel Y = ";
    str.append(aux);
    aux.setNum(a.accel_y);
    str.append(aux);
    aux = "\n  accel Z = ";
    str.append(aux);
    aux.setNum(a.accel_z);
    str.append(aux);
    ui->textEdit->setText(str);
}

/*!
 * \brief start selected kinect data flow
 */
void MainCore::on_pbGo_clicked()///--------------------------DEBUG
{
    ui->pbGo->setEnabled(false);
    int index = ui->combo->currentText().toInt();
    startK(index);
    currentKIndex = index;
    loop();
}
/*!
 * \brief stop kinect data flow and delete handler
 */
void MainCore::on_pbStop_clicked()
{
    ui->pbStop->setEnabled(false);
    stoploop();
    int index = ui->combo->currentText().toInt();
    if( index == currentKIndex ){
        stopK(index);
    }
}
/*!
 * \brief when combo item selected -> buttons activated
 * \param [in] arg1
 * pointer to string with kinect index selected in combo box
 */
void MainCore::on_combo_activated(const QString &arg1)
{
    int index = arg1.toInt();
    if ( index < 0 || index >= numDevices ){
        ui->textEdit->setText(" ERROR kinect index out of bounds. Restart.");
        return;
    }
    if( currentKIndex == -1 ){
        ui->pbGo->setEnabled(true);
        ui->pbStop->setEnabled(false);
    }else if( currentKIndex == index ){
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(true);
    }else{
        ui->textEdit->setText(" First stop running kinect, then start the other.");
    }
}

/*!
 * \brief start QTcpServer listening at port 9999 and connect to attendNewClient()
 */
void MainCore::startServer()
{
    qDebug("MainCore::startServer");
    if( !mainServer->listen(QHostAddress::Any,SRVKPORT) ){
        ui->textEdit->setText(mainServer->errorString());
        mainServer->close();
        return;
    }
    connect(mainServer, SIGNAL(newConnection()), this, SLOT(attendNewClient()));
}
/*!
 * \brief when client connection incoming create a new 'AttendClient' and bind
 */
void MainCore::attendNewClient()///------test with concurrent clients----------DEBUG
{
    qDebug("MainCore::attendNewClient");
    attendant = new AttendClient(mainServer->nextPendingConnection(),&structBuffers,this);
    if( attendant == NULL ) ui->textEdit->setText("BAD_ALLOC  AttendClient");
    attendClients.push_back(attendant);

    connect(attendant,SIGNAL(newSrvKinect(srvKinect)),this,SLOT(updateSrvKinect(srvKinect)));
}
