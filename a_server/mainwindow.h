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

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QMainWindow>
#include "apikinect/typekinect.h"
#include "apikinect/maincore.h"

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
    void srvKinectChanged(srvKinect newSrvK);
    void go();
    void stop();
    void

public slots:
    void videoDataReady();//print videoBuf
    void depthDataReady();//print depthBuf
    void barridoDataReady();//print BarridoBuf
    void barridoInit();//paint axes
    void updateKinect();//send ledOption and angle to kinect
    void updateSrvKinect(srvKinect newSrvK);//set new configuration data

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void init();
    void setServerIp();
    void putKcombo();

    void printTimeVector(std::vector<int> &timeV);

    //gui
    void on_pbGo_clicked();
    void on_pbStop_clicked();
  //  void on_combo_activated(const QString &arg1);
    void on_combo_highlighted(int index);

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

    int flag;//!< 0 stop loop(), otherwise let loop() run
    int numDevices;//!< number of detected kinects
    int currentDeviceIndex;//!< index of active kinect

    std::vector<int> timeVector;//msecs
};
#endif // MAINWINDOW_H
