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
}
/*!
 * \brief destructor
 */
MainWindow::~MainWindow()
{
    qDebug("MainWindow::~MainWindow()");
    if(apicore->getCurrentKIndex() !=-1){
//        stoploop();
//        stopK(currentKIndex);
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
void MainWindow::paintVideo()
{
    if( imgVideo != NULL ) delete imgVideo;
//    imgVideo = new QImage(videoBuf.data(),640,480,QImage::Format_RGB888);
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
void MainWindow::paintDepth()
{
    if( imgDepth != NULL ) delete imgDepth;
    imgDepth = new QImage(640,480,QImage::Format_RGB32);
    unsigned char r,g,b, distaChar;
    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
//            int value = depthBuf[(x+y*640)];//value is distance in mm
//            distaChar = value/39;//to transform distance to 8bit grey
//            r=g=b=distaChar;
//            imgDepth->setPixel(x,y,qRgb(r,g,b));// data to fit in 8 bits
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
void MainWindow::paintBarrido()
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
//        if(barridoBuf[i] == 0) continue;//no data info no plot
//        y = 235-(235*barridoBuf[i]/ui->tab_2->m_srvK.m_fZMax);//divide barridoBuf[i] by max mesured distance to fit in screen
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

}
/*!
 * \brief utility not implemented
 */
void MainWindow::paint2D()
{

}
/*!
 * \brief aux function to control time spend in calculus or painting.
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

void MainWindow::setSrvKinect(srvKinect newSrvK)
{
    //qDebug("MainWindow::setSrvKinect");
    QString auxStr;
    ui->le_limits_kbaseangle->setText(auxStr.setNum(newSrvK.m_fAngulo));
    ui->le_limits_kangle->setText(auxStr.setNum(newSrvK.m_iAnguloKinect));
    ui->le_limits_high->setText(auxStr.setNum(newSrvK.m_fAltura));
    ui->le_limits_Ymin->setText(auxStr.setNum(newSrvK.m_fYMin));
    ui->le_limits_Ymax->setText(auxStr.setNum(newSrvK.m_fYMax));
    ui->le_limits_Zmax->setText(auxStr.setNum(newSrvK.m_fZMax));
    ui->slider_D_refresh->setValue(newSrvK.m_ulRefresco3D);
    ui->slider_D_module->setValue(newSrvK.m_usModulo3D);
    if(newSrvK.m_bEnvio3D) ui->cb_3->setChecked(true);
    else ui->cb_3->setChecked(false);
    if(newSrvK.m_bEnvio2D) ui->cb_2->setChecked(true);
    else ui->cb_2->setChecked(false);
    if(newSrvK.m_bEnvioBarrido) ui->cb_barrido->setChecked(true);
    else ui->cb_barrido->setChecked(false);

}
/*!
 * \brief convenience function to initiate members
 */

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

void MainWindow::init()
{
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

    if( apicore->numKinects == 0 ){//num devices 0 => no kinect connected
        ui->combo->addItem("No kinect detected");
        ui->textEdit->setText(" No kinect detected, unable to start");
        ui->textEdit->show();
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(false);
    }else{
        for( int i = 0; i < apicore->numKinects ; i++){
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
    //connect line edit -> slider value
    connect(ui->le_video,SIGNAL(textChanged(QString)),this,SLOT(sliderVideoUp(QString)));
    connect(ui->le_depth,SIGNAL(textChanged(QString)),this,SLOT(sliderDepthUp(QString)));
    connect(ui->le_D_refresh,SIGNAL(textChanged(QString)),this,SLOT(slider3DUp(QString)));
    connect(ui->le_D_module,SIGNAL(textChanged(QString)),this,SLOT(sliderModuleUp(QString)));
    //connect slider -> line edit text
    connect(ui->slider_video,SIGNAL(sliderMoved(int)),ui->le_video,SLOT(setNum(int)));
    connect(ui->slider_depth,SIGNAL(sliderMoved(int)),ui->le_depth,SLOT(setNum(int)));
    connect(ui->slider_D_refresh,SIGNAL(sliderMoved(int)),ui->le_D_refresh,SLOT(setNum(int)));
    connect(ui->slider_D_module,SIGNAL(sliderMoved(int)),ui->le_D_module,SLOT(setNum(int)));
}
/*!
 * \brief start selected kinect data flow
 */
void MainWindow::on_pbGo_clicked()///--------------------------DEBUG
{
    ui->pbGo->setEnabled(false);
    int index = ui->combo->currentText().toInt();
//    startK(index);
    apicore->currentKIndex = index;
//    loop();
}
/*!
 * \brief stop kinect data flow and delete handler
 */
void MainWindow::on_pbStop_clicked()
{
    ui->pbStop->setEnabled(false);
//    stoploop();
    int index = ui->combo->currentText().toInt();
    if( index == apicore->currentKIndex ){
//        stopK(index);
    }
}
/*!
 * \brief when combo item selected -> buttons activated
 * \param [in] index
 * pointer to string with kinect index selected in combo box
 */
void MainWindow::on_combo_highlighted(int index)
{
    if ( index < 0 || index >= apicore->numKinects ){
        ui->textEdit->setText(" ERROR kinect index out of bounds. Restart.");
        return;
    }
    if( apicore->currentKIndex == -1 ){
        ui->pbGo->setEnabled(true);
        ui->pbStop->setEnabled(false);
    }else if( apicore->currentKIndex == index ){///Recuera apagar k activo antes de activar otro k
        ui->pbGo->setEnabled(false);
        ui->pbStop->setEnabled(true);
    }else{
        ui->textEdit->setText(" First stop running active kinect, then start the other one selected.");
    }
}
/*!
 * \brief utility function to link video slider and line edit
 * \param s
 */
void MainWindow::sliderVideoUp(QString s)
{
    int i = s.toInt();
    if( i == ui->slider_video->value() ) return;
    if( i< 33 ){
        i = 33;
        ui->le_video->setText(s.setNum(i));
    }
    if( i>2000 ){
        i=2000;
        ui->le_video->setText(s.setNum(i));
    }
    ui->le_video->setText(s.setNum(i));
    ui->slider_video->setSliderPosition(i);
}
/*!
 * \brief utility function to link depth slider and line edit
 * \param s
 */
void MainWindow::sliderDepthUp(QString s)
{
    int i = s.toInt();
    if( i == ui->slider_depth->value() ) return;
    if( i< 33 ){
        i = 33;
        ui->le_depth->setText(s.setNum(i));
    }
    if( i>2000 ){
        i=2000;
        ui->le_depth->setText(s.setNum(i));
    }
    ui->le_depth->setText(s.setNum(i));
    ui->slider_depth->setSliderPosition(i);
}
/*!
 * \brief utility function to link 3D slider and line edit
 * \param s
 */
void MainWindow::slider3DUp(QString s)
{
    int i = s.toInt();
    if( i == ui->slider_D_refresh->value() ) return;
    if( i< 33 ){
        i = 33;
        ui->le_D_refresh->setText(s.setNum(i));
    }
    if( i>2000 ){
        i=2000;
        ui->le_D_refresh->setText(s.setNum(i));
    }
    ui->le_D_refresh->setText(s.setNum(i));
    ui->slider_D_refresh->setSliderPosition(i);
}
/*!
 * \brief utility function to link module slider and line edit
 * \param s
 */
void MainWindow::sliderModuleUp(QString s)
{
    int i = s.toInt();
    if( i == ui->slider_D_module->value() ) return;
    if( i< 1 ){
        i = 1;
        ui->le_D_module->setText(s.setNum(i));
    }
    if( i>10 ){
        i=10;
        ui->le_D_module->setText(s.setNum(i));
    }
    ui->le_D_module->setText(s.setNum(i));
    ui->slider_D_module->setSliderPosition(i);
}
