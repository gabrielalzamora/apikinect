/*
 * Copyright (c) 2017  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */
#include "maincalib.h"

MainCalib::MainCalib(QObject *parent) : QObject(parent)
{
    init();
}

/*!
 * \brief destructor
 */
MainCalib::~MainCalib()
{
    qDebug("MainCalib::~MainCalib()");
}
/*!
 * \brief MainCalib::startK
 * \param indexK
 */
void MainCalib::startK(int indexK)
{
    qDebug("MainCalib::startK");
    device = &freenect.createDevice<Apikinect>(indexK);
    if(videoFormat){
        device->startVideo();
        timerVideo->start(34);//34 ms ~30 fps
    }else{
        device->startDepth();
        timerDepth->start(34);//34 ms ~30 fps
    }
    set_indexK(indexK);
}

void MainCalib::stopK(int indexK)
{
    qDebug("MainCalib::stopK");
    if(videoFormat){
        timerVideo->stop();
        device->stopVideo();
    }else{
        timerDepth->stop();
        device->stopDepth();
    }
    freenect.deleteDevice(indexK);
    set_indexK(-1);
    device = NULL;
}
/*!
 * \brief set led green and kinect angle =0º.
 */
void MainCalib::updateKinect()
{
    qDebug("MainCalib::updateKinect");
    if( device != NULL ){
        device->setLed(LED_BLINK_RED_YELLOW);
        device->setTiltDegrees(0.0);
    }
}

/*!
 * \brief getter for format
 * \return format
 */
bool MainCalib::get_format()
{
    //qDebug("MainCalib::set_format()");
    return videoFormat;
}
/*!
 * \brief setter for format
 * \param [in] formatSelected
 */
void MainCalib::set_format(bool format)
{
    qDebug("MainCalib::set_format()");
    videoFormat=format;
}
/*!
 * \brief getter for currentKIndex
 * \return currentKIndex
 */
int MainCalib::get_indexK()
{
    //qDebug("MainCalib::get_indexK()");
    return indexK;
}
/*!
 * \brief setter of currentKIndex
 * \param index
 */
void MainCalib::set_indexK(int index)
{
    //qDebug("MainCalib::set_indexK()");
    indexK = index;
}
/*!
 * \brief getter number of detected Kinects
 * \return number of kinect
 */
int MainCalib::get_numKinects()
{
    //qDebug("MainCalib::get_numKinects()");
    return numKinects;
}
/*!
 * \brief set current number of detected Kinects
 */
void MainCalib::set_numKinects(void)
{
    //qDebug("MainCalib::set_numKinects()");
    numKinects = freenect.deviceCount();
}

/*!
 * \brief convenience function to initiate members
 * reserve memory...
 */
void MainCalib::init()
{
    qDebug("MainCalib::init()");
    device = NULL;
    set_numKinects();
    set_indexK(-1);
    set_format(true);
    video.reserve(640*480*3);//video pixel r+g+b = 3*uint8_t
    depth.reserve(640*480*2);//depth pixel uint16_t = 2*uint8_t

    timerDepth = new QTimer(this);
    timerVideo = new QTimer(this);
    connect(timerDepth,SIGNAL(timeout()),this,SLOT(nextDepthFrame()));
    connect(timerVideo,SIGNAL(timeout()),this,SLOT(nextVideoFrame()));
}
/*!
 * \brief load next Video frame to Video vector
 */
void MainCalib::nextVideoFrame()
{
    //qDebug("MainCalib::nextVideoFrame");
    device->getRGB(video);
    emit printVideo();
}
/*!
 * \brief load next Depth frame to depth vector
 */
void MainCalib::nextDepthFrame()
{
    //qDebug("MainCalib::nextDepthFrame");
    device->getDepth(depth);
    emit printDepth();
}
