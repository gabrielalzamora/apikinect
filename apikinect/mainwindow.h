/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QTime>
#include <mutex>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTcpServer>
#include <QMainWindow>
#include "apikinect.h"
#include "attendclient.h"
#include "framegl.h"
#include "data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:

public slots:
    void videoDataReady();//print videoBuf
    void depthDataReady();//print depthBuf
    void barridoDataReady();//print BarridoBuf
    void barridoInit();//paint axes
    void updateKinect();//send ledOption and angle to kinect
    void updateSrvKinect(srvKinect newSrvK);//set new configuration data

private slots:
    void init();
    void setServerIp();
    void putKcombo();

    //freenect kinect
    void startK(int indexK);
    void stopK(int indexK);
    void loop();
    void stoploop();
    void printTimeVector(std::vector<int> &timeV);

    //gui
    void on_pbGo_clicked();
    void on_pbStop_clicked();
    void on_combo_activated(const QString &arg1);

    //server
    void startServer();
    void attendNewClient();

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private:
    Ui::MainWindow *ui;
    QGraphicsScene *sceneVideo;
    QGraphicsScene *sceneDepth;
    QGraphicsScene *sceneBarre;
    QImage *imgVideo;
    QImage *imgDepth;
    QImage *imgBarre;
    QGraphicsEllipseItem *ellipse;//!< holds single Barrido point to add to sceneBarre
    std::vector<QGraphicsEllipseItem*> ellipseVector;//!< holds Barrido points to paint in ellipse
    QPainter *ptpt;

    Freenect::Freenect freenect;//!< Freenect class object to start events thread and Devices
    freenect_context *context;//!< point to usb context associated to kinect data handling
    Apikinect *device;//!< class object that handle kinect sending led, angle orders, receiving frames, acceleration data

    std::vector<uint8_t> videoBuf;//!< container of video info from kinect
    std::vector<uint16_t> depthBuf;//!< container of depth info from kinect
    std::vector<point3c> p3Buf;//!< container of points cloud <- video+depth
    std::vector<point2> p2Buf;//!< container of 2D points = (point cloud) - color - y(coordinate)
    std::vector<uint32_t> barridoBuf;//!< barridoBuf contains distance on angle (360-i)/2 degrees, xOz plane
    accel a;//!< acceleration components x,y,z (y ~ 9,81 if m_iAnguloKinect=0)
    pBuf structBuffers;//!< to tell mainServer where to find data buffers
    int flag;//!< 0 stop loop(), otherwise let loop() run
    int numDevices;//!< number of detected kinects
    int currentDeviceIndex;//!< index of active kinect

    QTcpServer *mainServer;
    AttendClient *attendant;
    std::vector<AttendClient*> attendClients;//!< contain active AttendClient (allow to access them)

    std::vector<int> timeVector;//msecs
};
#endif // MAINWINDOW_H
