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
    scene->addPixmap(QPixmap::fromImage(*img).scaled(ui->gvImg->width(),ui->gvImg->height(),Qt::KeepAspectRatio));
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
    img = new QImage(640,480,QImage::Format_RGB888);
    unsigned char z;
    for(int i = 0; i < 640*480; i++){
        z = calib->depth[i]/39;//to transform distance to 8bit grey
        img->setPixel(i%640,i/640,qRgb(z,z,z));// data to fit in 8 bits
    }
    scene->addPixmap(QPixmap::fromImage(*img).scaled(ui->gvImg->width(),ui->gvImg->height(),Qt::KeepAspectRatio));
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

    //timerDepth = new QTimer(this);
    //timerVideo = new QTimer(this);

    //connect(timerDepth,SIGNAL(timeout()),this,SLOT(nextDepthFrame()));
    //connect(timerVideo,SIGNAL(timeout()),this,SLOT(nextVideoFrame()));
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
    connect(ui->rb_video,SIGNAL(toggled(bool)),calib,SLOT(set_format(bool)));
    //connect mainserver signals -> this slots
    connect(calib,SIGNAL(printVideo()),this,SLOT(paintVideo()));
    connect(calib,SIGNAL(printDepth()),this,SLOT(paintDepth()));
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

void MainWindow::on_rb_video_toggled(bool checked)
{
    qDebug("MainWindow::on_rb_video_toggled()");

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
        ui->rb_depth->setEnabled(false);
        ui->rb_video->setEnabled(false);
    }else{
        qDebug("STOP");
        calib->stopK(comboIndex);
        ui->combo->setEnabled(true);
        ui->rb_depth->setEnabled(true);
        ui->rb_video->setEnabled(true);
    }
}
