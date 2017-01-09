/*
 * Copyright (c) 2017  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */
#include <QGraphicsScene>
#include "mainwindow.h"
#include "ui_mainwindow.h"


/*!
 * \brief MainWindow::MainWindow
 * \param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    calib = new MainCalib(this);

    init();
    putKcombo();
    initConnects();
}

/*!
 * \brief destructor
 */
MainWindow::~MainWindow()
{
    qDebug("MainWindow::~MainWindow()");
    delete ui;
    //calib->deleteLater();
}

/*!
 * \brief draw video image
 *
 * This function is called to paint new image data (new frame or image available).
 * Is painted on ui->gvImg sceneImg using data from 'video' vector data.
 */
void MainWindow::paintVideo()
{
    if ( img != NULL ) delete img;
    img = new QImage(calib->video.data(),640,480,QImage::Format_RGB888);
    scene->addPixmap(QPixmap::fromImage(*img).scaled(ui->gvImg->width()-2,ui->gvImg->height()-2,Qt::KeepAspectRatio));
    ui->gvImg->show();
}
/*!
 * \brief draw depth image
 *
 * This function is called to paint new image data (new frame or image available).
 * Is painted on ui->gvImg sceneImg using data from 'depth' vector data.
 */
void MainWindow::paintDepth()
{
    if ( img != NULL ) delete img;
    img = new QImage(640,480,QImage::Format_RGB32);
    uint8_t z;
    for(int i = 0; i < 640*480; i++){
        int ocho = calib->depth[i] & 0xff;
        z =calib->depth[i]>>8;
        //qDebug(" esto es lo que tiene trabajar con bits %d ocho = %d",z,ocho);
        switch (calib->depth[i]>>8) {
        case 0:
            img->setPixel(i%640,i/640,qRgb(255,255-ocho,255-ocho));
            break;
        case 1:
            img->setPixel(i%640,i/640,qRgb(255,ocho,0));
            break;
        case 2:
            img->setPixel(i%640,i/640,qRgb(255-ocho,255,0));
            break;
        case 3:
            img->setPixel(i%640,i/640,qRgb(0,255,ocho));
            break;
        case 4:
            img->setPixel(i%640,i/640,qRgb(0,255-ocho,255));
            break;
        case 5:
            img->setPixel(i%640,i/640,qRgb(0,0,255-ocho));
            break;
        default:
            img->setPixel(i%640,i/640,qRgb(0,0,0));
            break;
        }
        qApp->processEvents();
    }
    scene->addPixmap(QPixmap::fromImage(*img).scaled(ui->gvImg->width()-2,ui->gvImg->height()-2,Qt::KeepAspectRatio));
    ui->gvImg->show();
}

void MainWindow::paintIR()
{
    if ( img != NULL ) delete img;
    img = new QImage(640,480,QImage::Format_RGB32);
    uint8_t pix;
    for( int i=0; i < 640*480; i++){
        pix=calib->video[i];
        img->setPixel(i%640,i/640,qRgb(pix,pix,pix));
    }
    scene->addPixmap(QPixmap::fromImage(*img).scaled(ui->gvImg->width()-2,ui->gvImg->height()-2,Qt::KeepAspectRatio));
    ui->gvImg->show();
}

/*!
 * \brief utility function to start members
 */
void MainWindow::init()
{
    qDebug("MainWindow::init()");
    scene = new QGraphicsScene(this);
    ui->gvImg->setScene(scene);
    img = NULL;
    ui->rb_video->setChecked(true);
}
/*!
 * \brief fill combo list with detected kinect index
 */
void MainWindow::putKcombo()
{
    //qDebug("MainWindow::putKcombo()");
    if( calib->get_numKinects() == 0 ){//num devices 0 => no kinect connected
        ui->combo->addItem("No kinect detected");
        ui->textBrowser->setText(" No kinect detected, connect one and restart.");
        ui->textBrowser->show();
    }else{
        for( int i = 0; i < calib->get_numKinects() ; i++){
            QString str;
            ui->combo->addItem(str.setNum(i));
        }
        ui->textBrowser->setText(" Select kinect in combo box to start\n1-click combo\n2-click device number in combo\n3-click Go");
    }
    ui->combo->setEnabled(true);
    ui->combo->setEditable(false);
    ui->pb_go->setEnabled(false);
}
/*!
 * \brief MainWindow::initConnects
 */
void MainWindow::initConnects()
{
    //qDebug("MainWindow::initConnects()");
    connect(ui->rb_video,SIGNAL(toggled(bool)),this,SLOT(format()));
    connect(ui->rb_depth,SIGNAL(toggled(bool)),this,SLOT(format()));
    connect(ui->rb_ir,SIGNAL(toggled(bool)),this,SLOT(format()));
    //connect mainserver signals -> this slots
    connect(calib,SIGNAL(printVideo()),this,SLOT(paintVideo()));
    connect(calib,SIGNAL(printDepth()),this,SLOT(paintDepth()));
    connect(calib,SIGNAL(printIR()),this,SLOT(paintIR()));
}
/*!
 * \brief MainWindow::format
 */
void MainWindow::format()
{
    //qDebug("MainWindow::format()");
    if( ui->rb_video->isChecked() ){
        calib->set_format(1);
    }else if( ui->rb_depth->isChecked() ){
        calib->set_format(2);
    }else calib->set_format(3);
}
/*!
 * \brief MainWindow::on_combo_activated
 * \param index
 */
void MainWindow::on_combo_activated(int index)
{
    //qDebug("MainWindow::on_combo_activated()");
    comboIndex = index;
    ui->pb_go->setEnabled(true);
    ui->combo->setEnabled(false);
}
/*!
 * \brief MainWindow::on_pb_go_Clicked
 */
void MainWindow::on_pb_go_clicked()
{
    qDebug("MainWindow::on_pb_go_clicked()");
    if( ui->pb_go->isChecked() ){
        qDebug("GO");
        calib->startK(comboIndex);
        calib->updateKinect();
        ui->combo->setEnabled(false);
        ui->rb_ir->setEnabled(false);
        ui->rb_depth->setEnabled(false);
        ui->rb_video->setEnabled(false);
    }else{
        qDebug("STOP");
        calib->stopK(comboIndex);
        ui->combo->setEnabled(true);
        ui->rb_ir->setEnabled(true);
        ui->rb_depth->setEnabled(true);
        ui->rb_video->setEnabled(true);
    }
}
