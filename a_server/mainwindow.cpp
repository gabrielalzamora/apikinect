/*
 * Copyright (c) 2016  Gabriel Alzamora
 * see CONTRIB file
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <QNetworkInterface>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "apikinect/maincore.h"

/*!
 * \class MainWindow
 * \brief gui to show apikinect functionality.
 *
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
//    numDevices = freenect.deviceCount();

    init();
    setServerIp();
    putKcombo();//fill combo box with available kinects
    paintBarridoAxes();//paint axes on barrido view
    initconnects();
    apiconnects();
}
/*!
 * \brief destructor
 */
MainWindow::~MainWindow()
{
    qDebug("MainWindow::~MainWindow()");
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
void MainWindow::paintVideo()
{
//    QTime t;
//    t.start();
    if( imgVideo != NULL ) delete imgVideo;
    imgVideo = new QImage(apicore->videoBuf.data(),640,480,QImage::Format_RGB888);
    sceneVideo->addPixmap(QPixmap::fromImage(*imgVideo).scaled(ui->gvVideo->width()-2,ui->gvVideo->height()-2,Qt::KeepAspectRatio));
    //sceneVideo->addPixmap(QPixmap::fromImage(*imgVideo).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvVideo->show();
//    apicore->timeVector[e_video] = t.elapsed();
}
/*!
 * \brief draw depth image
 *
 * This function is called to paint new depth data (new frame or image available).
 * Is painted on ui->gvDepth sceneDepth using data from 'depthBuf' vector data.
 */
void MainWindow::paintDepth()
{
//    QTime t;
//    t.start();
    if( imgDepth != NULL ) delete imgDepth;
    imgDepth = new QImage(640,480,QImage::Format_RGB32);
    unsigned char r,g,b, distaChar;
    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
            int value = apicore->depthBuf[(x+y*640)];//value is distance in mm
            distaChar = value/39;//to transform distance to 8bit grey
            r=g=b=distaChar;
            imgDepth->setPixel(x,y,qRgb(r,g,b));// data to fit in 8 bits
        }
    }
    sceneDepth->addPixmap(QPixmap::fromImage(*imgDepth).scaled(ui->gvDepth->width()-2,ui->gvDepth->height()-2,Qt::KeepAspectRatio));
    //sceneDepth->addPixmap(QPixmap::fromImage(*imgDepth).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvDepth->show();
//    apicore->timeVector[e_depth] = t.elapsed();
}
/*!
 * \brief draw Barrido
 *
 * This function is called when paint new Barrido data is needed.
 * Is painted on ui->gvBarrido sceneBarre using 'barridoBuf' vector data.
 */
void MainWindow::paintBarrido()
{
//    QTime t;
//    t.start();
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
        if(apicore->barridoBuf[i] == 0) continue;//no data info no plot
        y = 235-(235*apicore->barridoBuf[i]/getSrvKinect().m_fZMax);//scale barridoBuf[i] to fit in screen
        //y = 235-(235*apicore->barridoBuf[i]/ui->le_limits_Zmax->text().toInt());
        x = 320*(360-i)/360;
        ellipseVector.push_back(new QGraphicsEllipseItem(x,y,1.0,1.0));
        sceneBarre->addItem(ellipseVector[aux]);
        aux++;
    }
    ui->gvBarrido->show();
//    apicore->timeVector[e_barrido] = t.elapsed();
}
/*!
 * \brief draw axes on sceneBarre to show on gvBarrido.
 */
void MainWindow::paintBarridoAxes()
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
 * \brief utility not implemented
 */
void MainWindow::paint3D()
{
//    QTime t;
//    t.start();
    ui->glWidget->setCloud(apicore->p3rgbBuf);
    ui->glWidget->repaint();
//    apicore->timeVector[e_3] = t.elapsed();
}
/*!
 * \brief utility not implemented
 */
void MainWindow::paint2D()
{
//    QTime t;
//    t.start();
    ui->glWidget->setCloud(apicore->p2Buf);
    ui->glWidget->repaint();
//    apicore->timeVector[e_2] = t.elapsed();
}
/*!
 * \brief aux function to show time spent in calculus or painting.
 */
void MainWindow::printTimeVector()
{
    //qDebug("MainWindow::printTimeVector");
    QString str,aux;
    accel a(apicore->getAccel());
    str = "get video = ";
    aux.setNum(apicore->getTime(e_video));
    str.append(aux);
    aux = " \nget depth = ";
    str.append(aux);
    aux.setNum(apicore->getTime(e_depth));
    str.append(aux);
    aux = " \nget 3D = ";
    str.append(aux);
    aux.setNum(apicore->getTime(e_3));
    str.append(aux);
    aux = "\nget 2D = ";
    str.append(aux);
    aux.setNum(apicore->getTime(e_2));
    str.append(aux);
    aux = "\nget Barrido = ";
    str.append(aux);
    aux.setNum(apicore->getTime(e_barrido));
    str.append(aux);
    //pinta las aceleraciones----------------------accel
    aux = "\n  accel X = ";
    str.append(aux);
    aux.setNum(apicore->getAccel().accel_x);
    str.append(aux);
    aux = "\n  accel Y = ";
    str.append(aux);
    aux.setNum(apicore->getAccel().accel_y);
    str.append(aux);
    aux = "\n  accel Z = ";
    str.append(aux);
    aux.setNum(apicore->getAccel().accel_z);
    str.append(aux);
    ui->textEdit->setText(str);
}
/*!
 * \brief set ConfigData gui as newSrvK says
 * \param newSrvK
 */
void MainWindow::setSrvKinect(srvKinect newSrvK)
{
    //qDebug("MainWindow::setSrvKinect");
    QString auxStr;
    ui->le_limits_kbaseangle->setText(auxStr.setNum(newSrvK.m_fAngulo));
    ui->le_limits_kangle->setText(auxStr.setNum(newSrvK.m_iAnguloKinect));
    ui->le_limits_high->setText(auxStr.setNum(newSrvK.m_fAltura));
    ui->le_limits_Ymin->setText(auxStr.setNum(newSrvK.m_fYMin));
    ui->le_limits_Ymax->setText(auxStr.setNum(newSrvK.m_fYMax));
    ui->le_limits_Zmax->setText(auxStr.setNum(newSrvK.m_fZMax));//---------6/21

    ui->slider_D_refresh->setValue(newSrvK.m_ulRefresco3D);
    ui->slider_D_module->setValue(newSrvK.m_usModulo3D);
    if(newSrvK.m_bEnvio3D) ui->cb_D_3->setChecked(true);
    else ui->cb_D_3->setChecked(false);
    if(newSrvK.m_bEnvio2D) ui->cb_D_2->setChecked(true);
    else ui->cb_D_2->setChecked(false);
    if(newSrvK.m_bEnvioBarrido) ui->cb_D_barrido->setChecked(true);
    else ui->cb_D_barrido->setChecked(false);
    if(newSrvK.m_bCompress3D) ui->cb_D_comp->setChecked(true);
    else ui->cb_D_comp->setChecked(false);
    ui->le_D_ecu->setText(auxStr.setNum(newSrvK.m_iBarridoEcu));
    ui->le_D_Ymin->setText(auxStr.setNum(newSrvK.m_iBarridoYMin));
    ui->le_D_Ymax->setText(auxStr.setNum(newSrvK.m_iBarridoYMax));//-------15/21

    ui->slider_depth->setValue(newSrvK.m_ulRefrescoDepth);
    if(newSrvK.m_bEnvioDepth) ui->cb_depth->setChecked(true);
    else ui->cb_depth->setChecked(false);
    if(newSrvK.m_bCompressDepth) ui->cb_depth_comp->setChecked(true);
    else ui->cb_depth_comp->setChecked(false);//---------------------------18/21

    ui->slider_video->setValue(newSrvK.m_ulRefrescoColor);
    if(newSrvK.m_bEnvioColor) ui->cb_video->setChecked(true);
    else ui->cb_depth->setChecked(false);
    if(newSrvK.m_bCompressColor) ui->cb_video_comp->setChecked(true);
    else ui->cb_video_comp->setChecked(false);//---------------------------21/21
}
/*!
 * \brief return srvKinect obtained from ConfigData gui
 * \return srvKinect obtained from ConfigData gui
 */
srvKinect MainWindow::getSrvKinect()
{
    srvKinect srvK;
    srvK.m_fAngulo = ui->le_limits_kbaseangle->text().toDouble();
    srvK.m_iAnguloKinect = ui->le_limits_kangle->text().toInt();
    srvK.m_fAltura = ui->le_limits_high->text().toFloat();
    srvK.m_fYMin = ui->le_limits_Ymin->text().toFloat();
    srvK.m_fYMax = ui->le_limits_Ymax->text().toFloat();
    srvK.m_fZMax =  ui->le_limits_Zmax->text().toFloat();//-------------6/21

    srvK.m_ulRefresco3D = ui->slider_D_refresh->value();
    srvK.m_usModulo3D = ui->slider_D_module->value();
    srvK.m_bEnvio3D = ui->cb_D_3->isChecked();
    srvK.m_bEnvio2D = ui->cb_D_2->isChecked();
    srvK.m_bEnvioBarrido = ui->cb_D_barrido->isChecked();
    srvK.m_bCompress3D = ui->cb_D_comp->isChecked();
    srvK.m_iBarridoEcu = ui->le_D_ecu->text().toInt();
    srvK.m_iBarridoYMin = ui->le_D_Ymin->text().toInt();
    srvK.m_iBarridoYMax = ui->le_D_Ymax->text().toInt();//-------------15/21

    srvK.m_ulRefrescoDepth = ui->slider_depth->value();
    srvK.m_bEnvioDepth = ui->cb_depth_send->isChecked();
    srvK.m_bCompressDepth = ui->cb_depth_comp->isChecked();
    srvK.m_ulRefrescoColor = ui->slider_video->value();
    srvK.m_bEnvioColor = ui->cb_video_send->isChecked();
    srvK.m_bCompressColor = ui->cb_video_comp->isChecked();//----------21/21

    return srvK;
}

/*!
 * \brief override window close event to stop loop and delete apikinect handler...working
 * \param [in] event
 */
void MainWindow::closeEvent(QCloseEvent *event)
{
    qDebug("MainWindow::closeEvent()");
    if(apicore->getDeviceStatus()){
        apicore->~MainCore();
    }
    //exit(0);
}

/*!
 * \brief MainWindow::init
 */
void MainWindow::init()
{
    apicore = new MainCore(this);
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

    if( apicore->getKnumber() == 0 ){//num devices 0 => no kinect connected
        ui->combo->addItem("No kinect detected");
        ui->textEdit->setText(" No kinect detected, unable to start");
        ui->textEdit->show();
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(false);
    }else{
        for( int i = 0; i < apicore->getKnumber() ; i++){
            QString str;
            ui->combo->addItem(str.setNum(i));
        }
        ui->textEdit->setText(" Select kinect in combo box to start\n1-click combo\n2-click device number in combo\n3-click Go");
    }
}
/*!
 * \brief connect data tab_2 widgets
 */
void MainWindow::initconnects()
{
    //connect spin box -> slider value
    connect(ui->sb_video,SIGNAL(valueChanged(int)),this,SLOT(sliderVideoUp(int)));
    connect(ui->sb_depth,SIGNAL(valueChanged(int)),this,SLOT(sliderDepthUp(int)));
    connect(ui->sb_D_refresh,SIGNAL(valueChanged(int)),this,SLOT(slider3DUp(int)));
    connect(ui->sb_D_module,SIGNAL(valueChanged(int)),this,SLOT(sliderModuleUp(int)));
    //connect slider -> spin box value
    connect(ui->slider_video,SIGNAL(sliderMoved(int)),ui->sb_video,SLOT(setValue(int)));
    connect(ui->slider_depth,SIGNAL(sliderMoved(int)),ui->sb_depth,SLOT(setValue(int)));
    connect(ui->slider_D_refresh,SIGNAL(sliderMoved(int)),ui->sb_D_refresh,SLOT(setValue(int)));
    connect(ui->slider_D_module,SIGNAL(sliderMoved(int)),ui->sb_D_module,SLOT(setValue(int)));
}
/*!
 * \brief connect api signals to gui functions
 */
void MainWindow::apiconnects()
{
    //API
    //connect maincore signals -> this slots
    connect(apicore,SIGNAL(printVideo()),this,SLOT(paintVideo()));
    connect(apicore,SIGNAL(printDepth()),this,SLOT(paintDepth()));
    connect(apicore,SIGNAL(print3D()),this,SLOT(paint3D()));
    connect(apicore,SIGNAL(print2D()),this,SLOT(paint2D()));
    connect(apicore,SIGNAL(printBarrido()),this,SLOT(paintBarrido()));
    connect(apicore,SIGNAL(updateSrvKinect(srvKinect)),this,SLOT(setSrvKinect(srvKinect)));
    emit apicore->updateSrvKinect(apicore->getSrvKinect());
}
/*!
 * \brief start selected kinect data flow
 */
void MainWindow::on_pbGo_clicked()
{
    ui->pbGo->setEnabled(false);
    ui->pbStop->setEnabled(true);

    if( comboIndex != apicore->getCurrentKIndex() && apicore->getCurrentKIndex() != -1  ){
        apicore->stopK(apicore->getCurrentKIndex());
    }
    apicore->startK(comboIndex);
    apicore->setCurrentKIndex(comboIndex);
    apicore->go();
}
/*!
 * \brief stop kinect data flow and delete handler
 */
void MainWindow::on_pbStop_clicked()
{
    apicore->stop();
    ui->pbStop->setEnabled(false);
}
/*!
 * \brief when combo item activated -> buttons activated
 * \param [in] index
 */
void MainWindow::on_combo_activated(int index)
{
    //qDebug("MainWindow::on_combo_activated()");
    comboIndex = index;
    if ( comboIndex < 0 || comboIndex >= apicore->getKnumber() ){
        ui->textEdit->setText(" ERROR kinect index out of bounds. Restart.");
        return;
    }else if( apicore->getCurrentKIndex() == -1 || apicore->getCurrentKIndex() != index ){
        ui->pbGo->setEnabled(true);
    }else if( apicore->getCurrentKIndex() == index ){
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(true);
    }
}
/*!
 * \brief utility function to link video slider and line edit
 * \param s
 */
void MainWindow::sliderVideoUp(int i)
{
    if( i == ui->slider_video->value() ) return;
    if( i< 33 ){
        i = 33;
    }else if( i>2000 ){
        i=2000;
    }
    ui->sb_video->setValue(i);
    ui->slider_video->setSliderPosition(i);
}
/*!
 * \brief utility function to link depth slider and line edit
 * \param s
 */
void MainWindow::sliderDepthUp(int i)
{
    if( i == ui->slider_depth->value() ){
        return;
    }else if( i< 33 ){
        i = 33;
        ui->sb_depth->setValue(i);
        return;
    }else if( i>2000 ){
        i=2000;
        ui->sb_depth->setValue(i);
        return;
    }
    ui->slider_depth->setSliderPosition(i);
}
/*!
 * \brief utility function to link 3D slider and line edit
 * \param s
 */
void MainWindow::slider3DUp(int i)
{
    if( i == ui->slider_D_refresh->value() ){
        return;
    }else if( i< 33 ){
        i = 33;
        ui->sb_D_refresh->setValue(i);
        return;
    }else if( i>2000 ){
        i=2000;
        ui->sb_D_refresh->setValue(i);
        return;
    }
    ui->slider_D_refresh->setSliderPosition(i);
}
/*!
 * \brief utility function to link module slider and line edit
 * \param s
 */
void MainWindow::sliderModuleUp(int i)
{
    if( i == ui->slider_D_module->value() ){
        return;
    }if( i< 1 ){
        i = 1;
        ui->sb_D_module->setValue(i);
        return;
    }else if( i>10 ){
        i=10;
        ui->sb_D_module->setValue(i);
        return;
    }
    ui->slider_D_module->setSliderPosition(i);
}

