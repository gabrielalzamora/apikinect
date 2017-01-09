/*
 * Copyright (c) 2017  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */
#ifndef MAINCALIB_H
#define MAINCALIB_H

#include <QObject>
#include <QTimer>
#include "apikinect/apikinect2.h"

class MainCalib : public QObject
{
    Q_OBJECT
public:
    explicit MainCalib(QObject *parent = 0);
    ~MainCalib();
    std::vector<uint8_t> video;
    std::vector<uint16_t> depth;
    std::vector<uint8_t> ir;

signals:
    void printVideo();//warn gui there's a video frame ready to be painted
    void printDepth();
    void printIR();

public slots:
    //freenect kinect
    void startK(int indexK=0);
    void stopK(int indexK=0);
    void updateKinect();//set led green and angle kinect =0º
    //getters & setters
    int get_format();
    void set_format(int format);
    int get_indexK();
    void set_indexK(int index);
    int get_numKinects();
    void set_numKinects(void);

private slots:
    void init();
    void nextVideoFrame();// convenience function to use qtimers
    void nextDepthFrame();
    void nextIRFrame();

private:
    int videoFormat;// 3=ir, 2=depth, 1=video Kinect selected data format
    int numKinects;// number of detected kinects
    int indexK;// index of active kinect (indexK=-1 non active Kinect)
    Freenect::Freenect freenect;// Freenect class object to start events thread and Apikinect2
    freenect_context *context;// point to usb context associated to kinect data handling
    Apikinect2 *device;// object that handle kinect sending led, angle orders, receiving frames, acceleration data
    QTimer *timerVideo;
    QTimer *timerDepth;
    QTimer *timerIR;
};

#endif // MAINCALIB_H
