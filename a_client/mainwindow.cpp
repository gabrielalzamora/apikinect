/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include <QNetworkInterface>
#include <QTime>
#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \class MainWindow is Client Graphic User Interface
 *
 * Handle through connects MainClient to provide and receive
 * all functionality
 */

/*!
 * \brief constructor
 *
 * MainWindow constructor starts sockets and graphics
 * objects to show data on gui and inherit ui class
 * ( in mainwindow.ui ) that contain buttons, menus,
 *  text edit... to interact.
 * ui->tab_2 is programmed in Data class
 * \param parent
 * not used
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    client = new MainClient(this);

    init();
    paintBarridoAxes();
    initConnects();
    apiConnects();

}
/*!
 * \brief destructor
 * to free memory and close properly
 */
MainWindow::~MainWindow()
{
    delete ui;
    client->deleteLater();
    sceneVideo->deleteLater();
    sceneDepth->deleteLater();
    sceneBarre->deleteLater();
}

/*!
 * \brief draw video image
 *
 * This function is called to paint new video data (new frame or image available).
 * Is painted on ui->gvVideo sceneVideo using data from 'videoBuf' vector data.
 */
void MainWindow::paintVideo()
{
    if( !ui->cb_video->isChecked() ){//no pido pintar video no lo pinto
//        client->setTime(e_paint_video, 0);
        return;
    }
//    QTime t;
//    t.start();
    if( imgVideo != NULL ) delete imgVideo;
    imgVideo = new QImage(client->videoBuf.data(),RES_KINECT_VIDEO_W,RES_KINECT_VIDEO_H,QImage::Format_RGB888);
    sceneVideo->addPixmap(QPixmap::fromImage(*imgVideo).scaled(ui->gvVideo->width()-2,ui->gvVideo->height()-2,Qt::KeepAspectRatio));
    ui->gvVideo->show();
//    client->setTime(e_paint_video, t.elapsed());
}
/*!
 * \brief draw depth image
 *
 * This function is called to paint new depth data (new frame or image available).
 * Is painted on ui->gvDepth sceneDepth using data from 'depthBuf' vector data.
 */
void MainWindow::paintDepth()
{
    if( !ui->cb_depth->isChecked() ){
//        client->setTime(e_paint_depth, 0);
        return;
    }
//    QTime t;
//    t.start();
    if( imgDepth != NULL ) delete imgDepth;
    imgDepth = new QImage(640,480,QImage::Format_RGB32);
    unsigned char r,g,b, distaChar;
    for(int x = 0; x < 640; x++){
        for(int y = 0; y < 480; y++){
            int value = client->depthBuf[(x+y*640)];//value is distance in mm
            distaChar = value/39;//to transform distance to 8bit grey
            r=g=b=distaChar;
            imgDepth->setPixel(x,y,qRgb(r,g,b));// data to fit in 8 bits
        }
    }
    sceneDepth->addPixmap(QPixmap::fromImage(*imgDepth).scaled(ui->gvDepth->width()-2,ui->gvDepth->height()-2,Qt::KeepAspectRatio));
    ui->gvDepth->show();
//    client->setTime(e_paint_depth, t.elapsed());
}
/*!
 * \brief draw Barrido
 *
 * This function is called to paint new Barrido data.
 * Is painted on ui->gvBarrido sceneBarre using 'barridoBuf' vector data.
 */
void MainWindow::paintBarrido()
{
    if( !ui->cb_barrido->isChecked() ){
//        client->setTime(e_paint_barrido, 0);
        return;
    }
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
        if(client->barridoBuf[i] == 0) continue;//no data info no plot
        y = 235-(235*client->barridoBuf[i]/client->getSrvKinect().m_fZMax);//scale barridoBuf[i] to fit in screen
        x = 320*(360-i)/360;
        ellipseVector.push_back(new QGraphicsEllipseItem(x,y,1.0,1.0));
        sceneBarre->addItem(ellipseVector[aux]);
        aux++;
    }
    ui->gvBarrido->show();
//    client->setTime(e_paint_barrido, t.elapsed());
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
 * \brief draw 3D
 *
 * This function is called to paint new 3D data.
 * Is painted on ui->glWidget using 'p3rgbBuf' vector data.
 */
void MainWindow::paint3D()
{
    if( !ui->cb_3->isChecked() ){
//        client->setTime(e_paint_3, 0);
        return;
    }
//    QTime t;
//    t.start();
    ui->glWidget->setCloud(client->p3Buf);
    ui->glWidget->repaint();
//    client->setTime(e_paint_3, t.elapsed());
}
/*!
 * \brief draw 2D
 *
 * This function is called to paint new 2D data.
 * Is painted on ui->gvBarrido sceneBarre using 'p2Buf' vector data.
 */
void MainWindow::paint2D()
{
    if( !ui->cb_2->isChecked() ){
//        client->setTime(e_paint_2, 0);
        return;
    }
//    QTime t;
//    t.start();
    ui->glWidget->setCloud(client->p2Buf);
    ui->glWidget->repaint();
//    client->setTime(e_paint_2, t.elapsed());
}
/*!
 * \brief aux function to show time spent in calculus or painting.
 */
void MainWindow::paintTimeVector()
{
    //qDebug("MainWindow::paintTimeVector()");
    //pinta el tiempo para captar datos----------calcula
    QString str,aux;
    accel a(client->getAccel());
    str = "get video = ";
/*    aux.setNum(client->getTime(e_video));
    str.append(aux);
    aux = " \nget depth = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_depth));
    str.append(aux);
    aux = " \nget 3D = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_3));
    str.append(aux);
    aux = "\nget 2D = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_2));
    str.append(aux);
    aux = "\nget Barrido = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_barrido));
    str.append(aux);
    //pinta el tiempo para pintar datos-----------dibuja
    aux = "\npaint video = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_paint_video));
    str.append(aux);
    aux = " \npaint depth = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_paint_depth));
    str.append(aux);
    aux = " \npaint 3D = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_paint_3));
    str.append(aux);
    aux = "\npaint 2D = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_paint_2));
    str.append(aux);
    aux = "\npaint Barrido = ";
    str.append(aux);
//    aux.setNum(client->getTime(e_paint_barrido));
    str.append(aux);
*/    //pinta las aceleraciones----------------------accel
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
    ui->textEdit->show();
}
/*!
 * \brief set ConfigData gui as newSrvK says
 * \param[in] newSrvK config data to show on GUI
 */
void MainWindow::setSrvKinect(srvKinect newSrvK)//ConfigData => ui->tab_2
{
    //qDebug("MainWindow::setSrvKinect");
    QString auxStr;
    ui->le_limits_kbaseangle->setText(auxStr.setNum(newSrvK.m_fAngulo));
    ui->sb_limits_kangle->setValue(newSrvK.m_iAnguloKinect);
    ui->le_limits_high->setText(auxStr.setNum(newSrvK.m_fAltura));
    ui->sb_limits_Ymin->setValue(newSrvK.m_fYMin);
    ui->sb_limits_Ymax->setValue(newSrvK.m_fYMax);
    ui->sb_limits_Zmax->setValue(newSrvK.m_fZMax);//----------------6/21

    //ui->slider_D_refresh->setValue(newSrvK.m_ulRefresco3D);
    ui->sb_D_refresh->setValue(newSrvK.m_ulRefresco3D);
    //ui->slider_D_module->setValue(newSrvK.m_usModulo3D);
    ui->sb_D_module->setValue(newSrvK.m_usModulo3D);
    if(newSrvK.m_bEnvio3D) ui->cb_D_3->setChecked(true);
    else ui->cb_D_3->setChecked(false);
    if(newSrvK.m_bEnvio2D) ui->cb_D_2->setChecked(true);
    else ui->cb_D_2->setChecked(false);
    if(newSrvK.m_bEnvioBarrido) ui->cb_D_barrido->setChecked(true);
    else ui->cb_D_barrido->setChecked(false);
    if(newSrvK.m_bCompress3D) ui->cb_D_comp->setChecked(true);
    else ui->cb_D_comp->setChecked(false);
    ui->le_D_ecu->setText(auxStr.setNum(newSrvK.m_iBarridoEcu));
    ui->sb_D_Ymin->setValue(newSrvK.m_iBarridoYMin);
    ui->sb_D_Ymax->setValue(newSrvK.m_iBarridoYMax);//-------------15/21

    //ui->slider_depth->setValue(newSrvK.m_ulRefrescoDepth);
    ui->sb_depth->setValue(newSrvK.m_ulRefrescoDepth);
    if(newSrvK.m_bEnvioDepth) ui->cb_depth_send->setChecked(true);
    else ui->cb_depth_send->setChecked(false);
    if(newSrvK.m_bCompressDepth) ui->cb_depth_comp->setChecked(true);
    else ui->cb_depth_comp->setChecked(false);//-------------------18/21

    //ui->slider_video->setValue(newSrvK.m_ulRefrescoColor);
    ui->sb_video->setValue(newSrvK.m_ulRefrescoColor);
    if(newSrvK.m_bEnvioColor) ui->cb_video_send->setChecked(true);
    else ui->cb_video_send->setChecked(false);
    if(newSrvK.m_bCompressColor) ui->cb_video_comp->setChecked(true);
    else ui->cb_video_comp->setChecked(false);//-------------------21/21
    ui->tab_2->update();
}
/*!
 * \brief return srvKinect obtained from ConfigData gui
 * \return srvKinect obtained from ConfigData gui
 */
srvKinect MainWindow::getSrvKinect()//ui->tab_2 => ConfigData
{
    //qDebug("MainWindow::getSrvKinect()");
    srvKinect srvK;
    srvK.m_fAngulo = ui->le_limits_kbaseangle->text().toDouble();
    srvK.m_iAnguloKinect = ui->sb_limits_kangle->text().toInt();
    srvK.m_fAltura = ui->le_limits_high->text().toFloat();
    srvK.m_fYMin = ui->sb_limits_Ymin->text().toFloat();
    srvK.m_fYMax = ui->sb_limits_Ymax->text().toFloat();
    srvK.m_fZMax =  ui->sb_limits_Zmax->text().toFloat();//-------------6/21

    srvK.m_ulRefresco3D = ui->sb_D_refresh->value();
    srvK.m_usModulo3D = ui->sb_D_module->value();
    srvK.m_bEnvio3D = ui->cb_D_3->isChecked();
    srvK.m_bEnvio2D = ui->cb_D_2->isChecked();
    srvK.m_bEnvioBarrido = ui->cb_D_barrido->isChecked();
    srvK.m_bCompress3D = ui->cb_D_comp->isChecked();
    srvK.m_iBarridoEcu = ui->le_D_ecu->text().toInt();
    srvK.m_iBarridoYMin = ui->sb_D_Ymin->text().toInt();
    srvK.m_iBarridoYMax = ui->sb_D_Ymax->text().toInt();//-------------15/21

    srvK.m_ulRefrescoDepth = ui->sb_depth->value();
    srvK.m_bEnvioDepth = ui->cb_depth_send->isChecked();
    srvK.m_bCompressDepth = ui->cb_depth_comp->isChecked();
    srvK.m_ulRefrescoColor = ui->sb_video->value();
    srvK.m_bEnvioColor = ui->cb_video_send->isChecked();
    srvK.m_bCompressColor = ui->cb_video_comp->isChecked();//----------21/21

    return srvK;
}
/*!
 * \brief show system message on textEdit
 * \param[in] str message to show
 */
void MainWindow::printMessage(QString str)
{
    ui->textEdit->append(str);
}

/*!
 * \brief convenience function to init members
 * and reserve memory
 */
void MainWindow::init()
{
    ellipseVector.reserve(360);
    ellipseVector.resize(0);

    sceneVideo = new QGraphicsScene(this);
    sceneDepth = new QGraphicsScene(this);
    sceneBarre = new QGraphicsScene(this);

    ui->gvVideo->setScene(sceneVideo);
    ui->gvDepth->setScene(sceneDepth);
    ui->gvBarrido->setScene(sceneBarre);

    imgVideo = NULL;
    imgDepth = NULL;
    imgBarre = NULL;

    //ui->lineEdit  let it accept ip
    ui->lineEdit->setReadOnly(false);
    ui->lineEdit->setInputMask("000.000.000.000;-");
    //ui->buttons not enabled till ip is set
    ui->pbGo->setEnabled(false);
    ui->pbDepth->setEnabled(false);
    ui->pbVideo->setEnabled(false);
    ui->pb3D->setEnabled(false);
    ui->pb2D->setEnabled(false);
    ui->pbBarrido->setEnabled(false);
    ui->pbAccel->setEnabled(false);
    //let show all
    ui->cb_depth->setChecked(true);
    ui->cb_video->setChecked(true);
    ui->cb_3->setChecked(true);
    ui->cb_2->setChecked(true);
    ui->cb_barrido->setChecked(true);

}
/*!
 * \brief MainWindow::initConnects
 */
void MainWindow::initConnects()
{
    //qDebug("MainWindow::initconnects");
    //connect slider -> spin box value
    connect(ui->slider_video,SIGNAL(sliderMoved(int)),ui->sb_video,SLOT(setValue(int)));
    connect(ui->slider_depth,SIGNAL(sliderMoved(int)),ui->sb_depth,SLOT(setValue(int)));
    connect(ui->slider_D_refresh,SIGNAL(sliderMoved(int)),ui->sb_D_refresh,SLOT(setValue(int)));
    connect(ui->slider_D_module,SIGNAL(sliderMoved(int)),ui->sb_D_module,SLOT(setValue(int)));
    //connect spin box -> slider value
    connect(ui->sb_video,SIGNAL(valueChanged(int)),this,SLOT(sliderVideoUp(int)));
    connect(ui->sb_depth,SIGNAL(valueChanged(int)),this,SLOT(sliderDepthUp(int)));
    connect(ui->sb_D_refresh,SIGNAL(valueChanged(int)),this,SLOT(slider3DUp(int)));
    connect(ui->sb_D_module,SIGNAL(valueChanged(int)),this,SLOT(sliderModuleUp(int)));
    //connect data changed to signal
    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(upServerSrvK(int)));
    connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(setHost()));
}
/*!
 * \brief connect api signals to gui functions
 */
void MainWindow::apiConnects()
{
    //qDebug("MainWindow::apiconnects");
    //API
    //connect mainclient signals -> this slots
    connect(client,SIGNAL(printVideo()),this,SLOT(paintVideo()));
    connect(client,SIGNAL(printDepth()),this,SLOT(paintDepth()));
    connect(client,SIGNAL(print3D()),this,SLOT(paint3D()));
    connect(client,SIGNAL(print2D()),this,SLOT(paint2D()));
    connect(client,SIGNAL(printBarrido()),this,SLOT(paintBarrido()));
    connect(client,SIGNAL(printTimeVector()),this,SLOT(paintTimeVector()));
    connect(client,SIGNAL(updateSrvKinect(srvKinect)),this,SLOT(setSrvKinect(srvKinect)));
    //update GUI data with client data
    setSrvKinect(client->getSrvKinect());
}
/*!
 * \brief auxiliary function to read host written by user in GUI QLineEdit
 */
void MainWindow::setHost()
{
    ui->lineEdit->setText("192.168.1.3");///-----------------DEBUG
//    ui->lineEdit->setText("192.168.0.157");///-----------------DEBUG
//    ui->lineEdit->setText("127.0.0.1");///-----------------DEBUG
    hostAddr = QHostAddress(ui->lineEdit->text());
    ui->textEdit->setText("connect to server ip:");
    ui->textEdit->append(ui->lineEdit->text());
    ui->pbGo->setEnabled(true);
}

/*!
 * \brief start selected kinect data flow
 */
void MainWindow::on_pbGo_clicked()
{
    //qDebug("MainWindow::on_pbGo_clicked");
    if( ui->pbGo->isChecked() ){
        //qDebug("CONNECT");
        client->setHost(ui->lineEdit->text());
        client->initConnection();
        ui->lineEdit->setEnabled(false);
        ui->pbDepth->setEnabled(true);
        ui->pbVideo->setEnabled(true);
        ui->pb3D->setEnabled(true);
        ui->pb2D->setEnabled(true);
        ui->pbBarrido->setEnabled(true);
        ui->pbAccel->setEnabled(true);
    }else{
        //qDebug("DESCON");
        client->closeConnection();
        ui->lineEdit->setEnabled(true);
        ui->pbDepth->setEnabled(false);
        if( ui->pbDepth->isChecked() ){
            client->finalizeDepth();
            ui->pbDepth->setChecked(false);
        }
        ui->pbVideo->setEnabled(false);
        if( ui->pbVideo->isChecked() ){
            client->finalizeVideo();
            ui->pbVideo->setChecked(false);
        }
        ui->pb3D->setEnabled(false);
        if( ui->pb3D->isChecked() ){
            client->finalize3D();
            ui->pb3D->setChecked(false);
        }
        ui->pb2D->setEnabled(false);
        if( ui->pb2D->isChecked() ){
            client->finalize2D();
            ui->pb2D->setChecked(false);
        }
        ui->pbBarrido->setEnabled(false);
        if( ui->pbBarrido->isChecked() ){
            client->finalizeBarrido();
            ui->pbBarrido->setChecked(false);
        }
        ui->pbAccel->setEnabled(false);
        if( ui->pbAccel->isChecked() ){
            client->finalizeAccel();
            ui->pbAccel->setChecked(false);
        }
    }
}
/*!
 * \brief clicked once conect depth socket
 * second click disconnect
 */
void MainWindow::on_pbDepth_clicked()
{
    //qDebug("MainWindow::on_pbDepth_clicked()");
    if (!(ui->pbDepth->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalizeDepth();
    }
    else
    {
        qDebug("CON\n");
        client->initDepth();
    }
}
/*!
 * \brief clicked once conect video socket
 * second click disconnect
 */
void MainWindow::on_pbVideo_clicked()
{
    //qDebug("MainWindow::on_pbVideo_clicked()");
    if (!(ui->pbVideo->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalizeVideo();
    }
    else
    {
        qDebug("CON\n");
        client->initVideo();
    }
}
/*!
 * \brief clicked once conect 3D socket
 * second click disconnect
 */
void MainWindow::on_pb3D_clicked()
{
    //qDebug("MainWindow::on_pb3D_clicked()");
    if (!(ui->pb3D->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalize3D();
    }
    else
    {
        qDebug("CON\n");
        client->init3D();
    }
}
/*!
 * \brief clicked once conect 3D socket
 * second click disconnect
 */
void MainWindow::on_pb2D_clicked()
{
    //qDebug("MainWindow::on_pb2D_clicked()");
    if (!(ui->pb2D->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalize2D();
    }
    else
    {
        qDebug("CON\n");
        client->init2D();
    }
}
/*!
 * \brief clicked once conect Barrido socket
 * second click disconnect
 */
void MainWindow::on_pbBarrido_clicked()
{
    //qDebug("MainWindow::on_pbBarrido_clicked()");
    if (!(ui->pbBarrido->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalizeBarrido();
    }
    else
    {
        qDebug("CON\n");
        client->initBarrido();
    }
}
/*!
 * \brief clicked once conect Accel socket
 * second click disconnect
 */
void MainWindow::on_pbAccel_clicked()
{
    //qDebug("MainWindow::on_pbAccel_clicked()");
    if (!(ui->pbAccel->isChecked()))
    {
        qDebug("DISCON\n");
        client->finalizeAccel();
    }
    else
    {
        qDebug("CON\n");
        client->initAccel();
    }
}
/*!
 * \brief check acceptable input and setHost
 */
void MainWindow::on_lineEdit_editingFinished()
{
    if( ui->lineEdit->hasAcceptableInput() ) setHost();
    else ui->textEdit->setText("Error IP,\n escribe IP p.ej.\n 192.168.12.3");
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
/*!
 * \brief update client srvKinect ConfigData
 * \param[in] i
 */
void MainWindow::upServerSrvK(int i)
{
    //qDebug("MainWindow::upServerSrvK tab:%u",i);
    if(!i){
        //server->setIR(ui->cb_ir->isChecked());//IR do not work
        /* //LED info do not reach server
        if(ui->rb_green->isChecked()) i = 1;
        if(ui->rb_red->isChecked()) i = 2;
        if(ui->rb_yellow->isChecked()) i = 3;
        if(ui->rb_blinkG->isChecked()) i = 4;
        if(ui->rb_blinkRY->isChecked()) i = 6;
        server->setLed(i);
        */
        client->setGUISrvKinect(getSrvKinect());
    }
}


/*!
 * \brief aux function to debug ConfigData transmission
 * \param[in] srvK
 */
void MainWindow::showK(srvKinect srvK)
{
    qDebug("MainWindow::showK()");
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
