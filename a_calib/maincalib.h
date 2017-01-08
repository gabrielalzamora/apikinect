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
#include "apikinect/apikinect.h"

class MainCalib : public QObject
{
    Q_OBJECT
public:
    explicit MainCalib(QObject *parent = 0);
    ~MainCalib();
    std::vector<uint8_t> video;
    std::vector<uint16_t> depth;

signals:
    void printVideo();//warn gui there's a video frame ready to be painted
    void printDepth();

public slots:
    //freenect kinect
    void startK(int indexK=0);
    void stopK(int indexK=0);
    void updateKinect();//set led green and angle kinect =0º
    //getters & setters
    bool get_format();
    void set_format(bool format);
    int get_indexK();
    void set_indexK(int index);
    int get_numKinects();
    void set_numKinects(void);

private slots:
    void init();
    void nextVideoFrame();// convenience function to use qtimers
    void nextDepthFrame();// convenience function to use qtimers

private:
    bool videoFormat;// false=depth, true=video Kinect selected data format
    int numKinects;// number of detected kinects
    int indexK;// index of active kinect (indexK=-1 non active Kinect)
    Freenect::Freenect freenect;// Freenect class object to start events thread and Apikinect
    freenect_context *context;// point to usb context associated to kinect data handling
    Apikinect *device;// object that handle kinect sending led, angle orders, receiving frames, acceleration data
    QTimer *timerVideo;
    QTimer *timerDepth;
};

#endif // MAINCALIB_H
