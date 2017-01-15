/*
 * Copyright (c) 2017  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */
#include <QGraphicsScene>
#include <QInputDialog>
#include <QDir>
#include "mainwindow.h"
#include "ui_mainwindow.h"


/*!
 * \brief GUI to handel Calibration
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
    //qDebug("MainWindow::paintVideo()");
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
    //qDebug("MainWindow::paintDepth()");
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
/*!
 * \brief draw IR image
 *
 * This function is called to paint new image data (new frame or image available).
 * Is painted on ui->gvImg sceneImg using data from 'video' vector data.
 * IR info is from IR camera, same that provides depth info; but comes as USB video.
 */
void MainWindow::paintIR()
{
    //qDebug("MainWindow::paintIR()");
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
    //qDebug("MainWindow::init()");
    scene = new QGraphicsScene(this);
    ui->gvImg->setScene(scene);
    img = NULL;
    ui->pb_go->setEnabled(false);
    ui->rb_video->setChecked(true);
    ui->pb_select->setEnabled(false);
    ui->pb_save->setEnabled(false);
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
}
/*!
 * \brief connects act as controller on
 * Model-View-Controller architecture
 */
void MainWindow::initConnects()
{
    //qDebug("MainWindow::initConnects()");
    // ui signals -> this slots
    connect(ui->rb_video,SIGNAL(toggled(bool)),this,SLOT(format()));
    connect(ui->rb_depth,SIGNAL(toggled(bool)),this,SLOT(format()));
    connect(ui->rb_ir,SIGNAL(toggled(bool)),this,SLOT(format()));
    //connect  calib -> this slots
    connect(calib,SIGNAL(printVideo()),this,SLOT(paintVideo()));
    connect(calib,SIGNAL(printDepth()),this,SLOT(paintDepth()));
    connect(calib,SIGNAL(printIR()),this,SLOT(paintIR()));
    //connect mainwindow -> calib

}
/*!
 * \brief Set on MinCalib MainWindow::calib videoFormat
 * format: video/depth/IR
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
 * \brief save current image on GUI to file
 * \param videoFormat tipe o video format
 * video/depth/IR
 */
void MainWindow::saveImg(int videoFormat)
{
    qDebug("MainWindow::saveImg()");

    QDir dir(QDir::current());
    dir.cd("..");
    QString file, name;

    bool ok;
    switch (videoFormat) {
    case 1:
        dir.cd("calib_video");
        name = "video_";
        break;
    case 2:
        dir.cd("calib_depth");
        name = "depth_";
        break;
    case 3:
        dir.cd("calib_ir");
        name = "ir_";
        break;
    default:
        ui->textBrowser->append("ERROR MainWindow::saveImg() incorrect videoFormat");
        return;
    }

    name = QInputDialog::getText(this,
                                 tr("write file name"),
                                 tr("name"),
                                 QLineEdit::Normal,
                                 name,
                                 &ok);
    if (ok && !name.isEmpty()){
        file = dir.absolutePath()+"/"+name+".jpg";
        if( img->save(file) ) ui->textBrowser->append("Image saved");
        else ui->textBrowser->append("ERROR image not saved");
    }else ui->textBrowser->setText("file name ERROR");
}
/*!
 * \brief Set user selected Kinect index in combo
 * \param [in] index number of Kinect device selected
 */
void MainWindow::on_combo_activated(int index)
{
    //qDebug("MainWindow::on_combo_activated()");
    comboIndex = index;
    ui->pb_go->setEnabled(true);
    ui->combo->setEnabled(false);
}
/*!
 * \brief Start Kinect device with index set in combo
 * clicked twice stop device and destroy its handler
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
        ui->pb_save->setEnabled(true);
        //ui->pb_select->setEnabled(true);
    }else{
        qDebug("STOP");
        calib->stopK(comboIndex);
        ui->combo->setEnabled(true);
        ui->rb_ir->setEnabled(true);
        ui->rb_depth->setEnabled(true);
        ui->rb_video->setEnabled(true);
        ui->pb_save->setEnabled(true);
        //ui->pb_select->setEnabled(false);
    }
}
/*!
 * \brief Save current image to file,
 * first stop image refresh to select
 * current QImage to save.
 */
void MainWindow::on_pb_save_clicked()
{
    qDebug("MainWindow::on_pb_save_clicked()");
    ui->pb_save->setEnabled(false);
    switch (calib->get_format()) {
        case 1:
            if(calib->timerVideo->isActive()){
                calib->timerVideo->stop();
                saveImg(1);
                calib->timerVideo->start();
            }else{ saveImg(1);}
            break;
        case 2:
            if(calib->timerDepth->isActive()){
                calib->timerDepth->stop();
                saveImg(2);
                calib->timerDepth->start();
            }else{ saveImg(2);}
            break;
        case 3:
            if(calib->timerIR->isActive()){
                calib->timerIR->stop();
                saveImg(3);
                calib->timerIR->start();

            }else { saveImg(3);}
            break;
        default:
            break;
    }
    ui->pb_save->setEnabled(true);
    return;
}
