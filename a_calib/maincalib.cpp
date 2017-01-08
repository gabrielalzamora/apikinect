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
    device->startVideo();
    device->startDepth();

    set_indexK(indexK);
}

void MainCalib::stopK(int indexK)
{
    qDebug("MainCalib::stopK");
    device->stopDepth();
    device->stopVideo();
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
int MainCalib::get_format()
{
    //qDebug("MainCalib::set_format()");
    return format;
}
/*!
 * \brief setter for format
 * \param [in] formatSelected
 */
void MainCalib::set_format(bool videoFormat)
{
    qDebug("MainCalib::set_format()");
    format=videoFormat;
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
}
