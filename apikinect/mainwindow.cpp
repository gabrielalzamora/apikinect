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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apikinect.h"

//#define SRVKPORT 10003

/*!
 * \class MainWindow
 * \brief holds all server functionality.
 *
 * Through members that are type Apikinect, AttendClient,
 * Ui_MainWindow (ui), Data (ui->tab_2),FrameGL (ui->glWidget),
 * QTcpServer and Freenect::Freenect manage kinect and client
 * relations.
 * Apikinect allow to handle and comunicate with kinect.
 * Ui_MainWindow (ui) is the graphic interface.
 * Data (ui->tab_2) holds configuration to handle data.
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
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    numDevices = freenect.deviceCount();

    init();
    startServer();
    setServerIp();
    putKcombo();//fill combo box with available kinects
    barridoInit();//paint axes on barrido view

    connect(ui->tab_2,SIGNAL(dataChanged()),this,SLOT(updateKinect()));
    connect(ui->tab_2,SIGNAL(ledOptionChanged()),this,SLOT(updateKinect()));
}
/*!
 * \brief destructor
 */
MainWindow::~MainWindow()
{
    qDebug("MainWindow::~MainWindow()");
    if(currentDeviceIndex !=-1){
        stoploop();
        stopK(currentDeviceIndex);
    }
    delete sceneVideo;
    delete sceneDepth;
    delete sceneBarre;
    if( imgVideo != NULL ) delete imgVideo;
    if( imgDepth != NULL ) delete imgDepth;
    if( imgBarre != NULL ) delete imgBarre;
    delete ui;
}
/*!
 * \brief draw video image
 *
 * This function is called to paint new video data (new frame or image available).
 * Is painted on ui->gvVideo sceneVideo using data from 'videoBuf' vector data.
 */
void MainWindow::videoDataReady()
{
    if( imgVideo != NULL ) delete imgVideo;
    imgVideo = new QImage(videoBuf.data(),640,480,QImage::Format_RGB888);
    sceneVideo->addPixmap(QPixmap::fromImage(*imgVideo).scaled(ui->gvVideo->width()-2,ui->gvVideo->height()-2,Qt::KeepAspectRatio));
    //sceneVideo->addPixmap(QPixmap::fromImage(*imgVideo).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvVideo->show();
}
/*!
 * \brief draw depth image
 *
 * This function is called to paint new depth data (new frame or image available).
 * Is painted on ui->gvDepth sceneDepth using data from 'depthBuf' vector data.
 */
void MainWindow::depthDataReady()
{
    if( imgDepth != NULL ) delete imgDepth;
    imgDepth = new QImage(640,480,QImage::Format_RGB32);
    unsigned char r,g,b, distaChar;
    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
            int value = depthBuf[(x+y*640)];//value is distance in mm
            distaChar = value/39;//to transform distance to 8bit grey
            r=g=b=distaChar;
            imgDepth->setPixel(x,y,qRgb(r,g,b));// data to fit in 8 bits
        }
    }
    sceneDepth->addPixmap(QPixmap::fromImage(*imgDepth).scaled(ui->gvDepth->width()-2,ui->gvDepth->height()-2,Qt::KeepAspectRatio));
    //sceneDepth->addPixmap(QPixmap::fromImage(*imgDepth).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvDepth->show();
}
/*!
 * \brief draw Barrido
 *
 * This function is called when paint new Barrido data is needed.
 * Is painted on ui->gvBarrido sceneBarre using 'barridoBuf' vector data.
 */
void MainWindow::barridoDataReady()
{
    int x,y;
    int aux(0);
    if( ellipseVector.size() != 0 ){
        for(int i=0;i<ellipseVector.size();i++){
            sceneBarre->removeItem(ellipseVector[i]);
            delete ellipseVector[i];
        }
        ellipseVector.resize(0);
    }
    for(int i=0;i<360;i++){
        if(barridoBuf[i] == 0) continue;//no data info no plot
        y = 235-(235*barridoBuf[i]/ui->tab_2->m_srvK.m_fZMax);//divide barridoBuf[i] by max mesured distance to fit in screen
        x = 320*(360-i)/360;
        ellipseVector.push_back(new QGraphicsEllipseItem(x,y,1.0,1.0));
        sceneBarre->addItem(ellipseVector[aux]);
        aux++;
    }
    ui->gvBarrido->show();
}
/*!
 * \brief draw axes on sceneBarre to show on gvBarrido.
 */
void MainWindow::barridoInit()
{
    ui->gvBarrido->setBackgroundBrush(QColor(200,240,240,255));//light blue
    sceneBarre->setBackgroundBrush(QColor(200,240,240,255));
    QPen ejesPen = QPen(QColor(255,0,0,255));//red color
    ejesPen.setWidth(2);
    QLine ejex = QLine(5,230,315,230);
    QLine ejey = QLine(160,230,160,5);
    sceneBarre->addLine(ejex,ejesPen);
    sceneBarre->addLine(ejey,ejesPen);
}
/*!
 * \brief set current led option and kinect angle on active kinect.
 *
 * When any data on 'Data' object change it sends the order to
 * update to active kinect
 */
void MainWindow::updateKinect()
{
    if( device != NULL ){
        device->setLed(freenect_led_options(ui->tab_2->ledOption));
        device->setTiltDegrees(double(ui->tab_2->m_srvK.m_iAnguloKinect));
    }
}
/*!
 * \brief set srvKinect data sended by client
 * \param [in] sk
 * client current srvKinect
 */
void MainWindow::updateSrvKinect(srvKinect newSrvK)
{
//    qDebug("MainServer::updateKinect");
    ui->tab_2->m_srvK.m_fAngulo = newSrvK.m_fAngulo;
    ui->tab_2->setLimitsLineEAngulo(newSrvK.m_fAngulo);
    ui->tab_2->m_srvK.m_iAnguloKinect = newSrvK.m_iAnguloKinect;
    ui->tab_2->setLimitsLineEAngK(newSrvK.m_iAnguloKinect);
    ui->tab_2->m_srvK.m_fAltura = newSrvK.m_fAltura;
    ui->tab_2->setLimitsLineEAltura(newSrvK.m_fAltura);
    ui->tab_2->m_srvK.m_fYMin = newSrvK.m_fYMin;
    ui->tab_2->setLimitsLineEYmin(newSrvK.m_fYMin);
    ui->tab_2->m_srvK.m_fYMax = newSrvK.m_fYMax;
    ui->tab_2->setLimitsLineEYmax(newSrvK.m_fYMax);
    ui->tab_2->m_srvK.m_fZMax = newSrvK.m_fZMax;
    ui->tab_2->setLimitsLineEZmax(newSrvK.m_fZMax);

    ui->tab_2->m_srvK.m_ulRefresco3D = newSrvK.m_ulRefresco3D;
    ui->tab_2->setPointsSlider(newSrvK.m_ulRefresco3D);
    ui->tab_2->m_srvK.m_usModulo3D = newSrvK.m_usModulo3D;
    ui->tab_2->setPointsSliderM(newSrvK.m_usModulo3D);

    ui->tab_2->m_srvK.m_bEnvio3D = newSrvK.m_bEnvio3D;
    ui->tab_2->setPointsCBenvio3D(newSrvK.m_bEnvio3D);
    ui->tab_2->m_srvK.m_bEnvio2D = newSrvK.m_bEnvio2D;
    ui->tab_2->setPointsCBenvio2(newSrvK.m_bEnvio2D);
    ui->tab_2->m_srvK.m_bEnvioBarrido = newSrvK.m_bEnvioBarrido;
    ui->tab_2->setPointsCBenvioB(newSrvK.m_bEnvioBarrido);
    ui->tab_2->m_srvK.m_bCompress3D = newSrvK.m_bCompress3D;
    ui->tab_2->setPointsCBcomprimido(newSrvK.m_bCompress3D);
    ui->tab_2->m_srvK.m_iBarridoEcu = newSrvK.m_iBarridoEcu;
    ui->tab_2->setPointsLineEEcu(newSrvK.m_iBarridoEcu);
    ui->tab_2->m_srvK.m_iBarridoYMin = newSrvK.m_iBarridoYMin;
    ui->tab_2->setPointsLineEYmin(newSrvK.m_iBarridoYMin);
    ui->tab_2->m_srvK.m_iBarridoYMax = newSrvK.m_iBarridoYMax;
    ui->tab_2->setPointsLineEYmax(newSrvK.m_iBarridoYMax);

    ui->tab_2->m_srvK.m_ulRefrescoDepth = newSrvK.m_ulRefrescoDepth;
    ui->tab_2->setDepthSlider(newSrvK.m_ulRefrescoDepth);
    ui->tab_2->m_srvK.m_bEnvioDepth = newSrvK.m_bEnvioDepth;
    ui->tab_2->setDepthCBenvio(newSrvK.m_bEnvioDepth);
    ui->tab_2->m_srvK.m_bCompressDepth = newSrvK.m_bCompressDepth;
    ui->tab_2->setDepthCBcomprimido(newSrvK.m_bCompressDepth);
    ui->tab_2->m_srvK.m_ulRefrescoColor = newSrvK.m_ulRefrescoColor;
    ui->tab_2->setVideoSlider(newSrvK.m_ulRefrescoColor);
    ui->tab_2->m_srvK.m_bEnvioColor = newSrvK.m_bEnvioColor;
    ui->tab_2->setVideoCBenvio(newSrvK.m_bEnvioColor);
    ui->tab_2->m_srvK.m_bCompressColor = newSrvK.m_bCompressColor;
    ui->tab_2->setVideoCBcomprimido(newSrvK.m_bCompressColor);

    emit updateKinect();
}

/*!
 * \brief convenience function to initiate members
 */
void MainWindow::init()
{
    //apikinect
    device = NULL;
    currentDeviceIndex = -1;
    flag = false;
    videoBuf.resize(640*480*3);
    depthBuf.resize(640*480);
    p3Buf.reserve(300000);//max number of points
    p3Buf.resize(0);//initially we have none
    p2Buf.reserve(300000);
    p2Buf.resize(0);
    barridoBuf.resize(360);
    a.accel_x = a.accel_y = a.accel_z = 0;
    structBuffers.ptrVideoBuf = &videoBuf;
    structBuffers.ptrDepthBuf = &depthBuf;
    structBuffers.ptrP3Buf = &p3Buf;
    structBuffers.ptrP2Buf = &p2Buf;
    structBuffers.ptrBarridoBuf = &barridoBuf;
    structBuffers.ptrAccel = &a;
    ellipseVector.reserve(360);
    ellipseVector.resize(0);
    sceneVideo = new QGraphicsScene;
    sceneDepth = new QGraphicsScene;
    sceneBarre = new QGraphicsScene;
    imgVideo = NULL;
    imgDepth = NULL;
    imgBarre = NULL;
    ui->gvVideo->setScene(sceneVideo);
    ui->gvDepth->setScene(sceneDepth);
    ui->gvBarrido->setScene(sceneBarre);
    timeVector.resize(e_xtra);
    //server
    mainServer = new QTcpServer(this);
}
/*!
 * \brief write my server ip on gui label
 */
void MainWindow::setServerIp()
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
    ui->label_ip->setText(ip);
}
/*!
 * \brief fill combo list with detected kinect index
 */
void MainWindow::putKcombo()
{
    if( numDevices == 0 ){//num devices 0 => no kinect connected
        ui->combo->addItem("No kinect detected");
        ui->textEdit->setText(" No kinect detected, unable to start");
        ui->textEdit->show();
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(false);
    }else{
        for( int i = 0; i < numDevices ; i++){
            QString str;
            ui->combo->addItem(str.setNum(i));
        }
        ui->textEdit->setText(" Select kinect in combo box to start\n1-click combo\n2-click device number in combo\n3-click Go");
    }
}

/*!
 * \brief create kinect handler.
 *
 * init 'Apikinect device' to handle kinect of indexK.
 * \param [in] indexK.
 */
void MainWindow::startK(int indexK)
{
    device = &freenect.createDevice<Apikinect>(indexK);
    device->startVideo();
    device->startDepth();
    currentDeviceIndex = indexK;
}
/*!
 * \brief destroy kinect handler.
 * \param [in] index kinect handler index to be destroyed.
 */
void MainWindow::stopK(int indexK)
{
    device->stopDepth();
    device->stopVideo();
    freenect.deleteDevice(indexK);
    currentDeviceIndex = -1;
    device = NULL;
}
/*!
 * \brief working horse retrieving video & depth info.
 *
 * Get video and depth info to pour it into buffers as
 * p3Buf (point cloud: 3D+color)...
 */
void MainWindow::loop()
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

        if( ui->tab_2->m_srvK.m_bEnvioColor ){
            t.start();//---------------------------------------------time.start
            device->getRGB(videoBuf);
            timeVector[e_video]=t.elapsed();//---------------------timeVector[e_video]
            if(ui->cb_video->isChecked())
                videoDataReady();//paint video on gvVideo
        }

        if( ui->tab_2->m_srvK.m_bEnvioDepth )
            t.start();//---------------------------------------------time.start
            device->getDepth(depthBuf);
            timeVector[e_depth]=t.elapsed();//---------------------timeVector[e_depth]
            if(ui->cb_depth->isChecked())
                depthDataReady();//paint depth on gvDepth

//        qApp->processEvents();//stay responsive to button click

        if( ui->tab_2->m_srvK.m_bEnvio2D && ui->tab_2->m_srvK.m_bEnvio3D && ui->tab_2->m_srvK.m_bEnvioBarrido){//all buffers
            //qDebug("  2D+3D+Barrido");
            t.start();//---------------------------------------------time.start
            device->getAll(&structBuffers,&ui->tab_2->m_srvK);//---get all points 3d, 2d & barrido
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_2->isChecked())
                ui->glWidget->setCloud(p2Buf);//send new point cloud to FrameGL
            if(ui->cb_3->isChecked())
                ui->glWidget->setCloud(p3Buf);
            if(ui->cb_2->isChecked() || ui->cb_3->isChecked())
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(!(ui->tab_2->m_srvK.m_bEnvio2D) && ui->tab_2->m_srvK.m_bEnvio3D && ui->tab_2->m_srvK.m_bEnvioBarrido){
            //qDebug("  3D+Barrido");
            t.start();//---------------------------------------------time.start
            device->get3dBarrido(&structBuffers,&ui->tab_2->m_srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_3->isChecked()){
                ui->glWidget->setCloud(p3Buf);//send FrameGL new point cloud
                ui->glWidget->repaint();//paint points cloud on oglwidget through FrameGL
            }
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(ui->tab_2->m_srvK.m_bEnvio2D && !(ui->tab_2->m_srvK.m_bEnvio3D) && ui->tab_2->m_srvK.m_bEnvioBarrido){
            //qDebug("  2D+Barrido");
            t.start();//---------------------------------------------time.start
            device->get2dBarrido(&structBuffers,&ui->tab_2->m_srvK);
            timeVector[e_2] = t.elapsed();//---------------------timeVector[e_2]
            if(ui->cb_2->isChecked()){
                ui->glWidget->setCloud(p2Buf);
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            }
            if(ui->cb_barrido->isChecked())
                barridoDataReady();//paint Barrido (swept)
        }else if(ui->tab_2->m_srvK.m_bEnvio2D && ui->tab_2->m_srvK.m_bEnvio3D && !(ui->tab_2->m_srvK.m_bEnvioBarrido)){
            //qDebug("  2D+3D");
            t.start();//---------------------------------------------time.start
            device->get2and3(&structBuffers,&ui->tab_2->m_srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_2->isChecked())
                ui->glWidget->setCloud(p2Buf);//send new point cloud to FrameGL
            if(ui->cb_3->isChecked())
                ui->glWidget->setCloud(p3Buf);
            if(ui->cb_2->isChecked() || ui->cb_3->isChecked())
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
        }else if(ui->tab_2->m_srvK.m_bEnvio3D){
            //qDebug("  3D");
            t.start();//---------------------------------------------time.start
            device->get3d(&structBuffers,&ui->tab_2->m_srvK);
            timeVector[e_3] = t.elapsed();//---------------------timeVector[e_3]
            if(ui->cb_3->isChecked()){
                ui->glWidget->setCloud(p3Buf);//send FrameGL new point cloud
                ui->glWidget->repaint();//paint points cloud on oglwidget through FrameGL
            }
        }else if(ui->tab_2->m_srvK.m_bEnvio2D){
            //qDebug("  2D");
            t.start();//---------------------------------------------time.start
            device->get2(&structBuffers,&ui->tab_2->m_srvK);
            timeVector[e_2] = t.elapsed();//---------------------timeVector[e_2]
            if(ui->cb_2->isChecked()){
                ui->glWidget->setCloud(p2Buf);
                ui->glWidget->repaint();//paint points clouds on oglwidget through FrameGL
            }
        }else if(ui->tab_2->m_srvK.m_bEnvioBarrido){
            //qDebug("  Barrido");
            t.start();//---------------------------------------------time.start
            device->getBarrido(&structBuffers,&ui->tab_2->m_srvK);
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
void MainWindow::stoploop()
{
    flag = 0;
}
/*!
 * \brief aux function to control time spend in calculus or painting.
 * \param [out] timeV vector to save data.
 */
void MainWindow::printTimeVector(std::vector<int> &timeV)
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
void MainWindow::on_pbGo_clicked()///--------------------------DEBUG
{
    ui->pbGo->setEnabled(false);
    int index = ui->combo->currentText().toInt();
    startK(index);
    currentDeviceIndex = index;
    loop();
}
/*!
 * \brief stop kinect data flow and delete handler
 */
void MainWindow::on_pbStop_clicked()
{
    ui->pbStop->setEnabled(false);
    stoploop();
    int index = ui->combo->currentText().toInt();
    if( index == currentDeviceIndex ){
        stopK(index);
    }
}
/*!
 * \brief when combo item selected -> buttons activated
 * \param [in] arg1
 * pointer to string with kinect index selected in combo box
 */
void MainWindow::on_combo_activated(const QString &arg1)
{
    int index = arg1.toInt();
    if ( index < 0 || index >= numDevices ){
        ui->textEdit->setText(" ERROR kinect index out of bounds. Restart.");
        return;
    }
    if( currentDeviceIndex == -1 ){
        ui->pbGo->setEnabled(true);
        ui->pbStop->setEnabled(false);
    }else if( currentDeviceIndex == index ){
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(true);
    }else{
        ui->textEdit->setText(" First stop running kinect, then start the other.");
    }
}

/*!
 * \brief start QTcpServer listening at port 9999 and connect to attendNewClient()
 */
void MainWindow::startServer()
{
    qDebug("MainWindow::startServer");
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
void MainWindow::attendNewClient()///------test with concurrent clients----------DEBUG
{
    qDebug("MainWindow::attendNewClient");
    attendant = new AttendClient(mainServer->nextPendingConnection(),&structBuffers,this);
    if( attendant == NULL ) ui->textEdit->setText("BAD_ALLOC  AttendClient");
    attendClients.push_back(attendant);

    connect(attendant,SIGNAL(newSrvKinect(srvKinect)),this,SLOT(updateSrvKinect(srvKinect)));
}

/*!
 * \brief override window close event to stop loop and delete apikinect handler.
 * \param [in] event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug("MainWindow::closeEvent()");
    //this->~MainWindow();//no es aquí, mira QTcpServer o sockets, peta si cierras tras conectar
    //exit(0);
}
