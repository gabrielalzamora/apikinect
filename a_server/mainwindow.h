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

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include "apikinect/mainserver.h"

class QGraphicsScene;
class QGraphicsItem;

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
    void ledChanged(int lop);
    void irChanged(int i);
    void go(int kindex);
    void stop(int kindex);

public slots:
    void paintVideo();
    void paintDepth();
    void paintBarrido();
    void paintBarridoAxes();
    void paint3D();
    void paint2D();
    void printTimeVector();
    void setSrvKinect(srvKinect newSrvK);//ConfigData => ui->tab_2
    srvKinect getSrvKinect();//ui->tab_2 => ConfigData

private slots:
    void init();
    void setServerIp();
    void putKcombo();
    void initconnects();
    //gui interactions
    void apiconnects();
    void on_pbGo_clicked();
    void on_combo_activated(int index);
    //data link in tab_2
    void sliderVideoUp(int i);
    void sliderDepthUp(int i);
    void slider3DUp(int i);
    void sliderModuleUp(int i);
    void upServerSrvK(int i);
    //apikinect

private:
    Ui::MainWindow *ui;
    int comboIndex;
    QGraphicsScene *sceneVideo;
    QGraphicsScene *sceneDepth;
    QGraphicsScene *sceneBarre;
    QImage *imgVideo;
    QImage *imgDepth;
    QImage *imgBarre;
    QGraphicsEllipseItem *ellipse;// holds single Barrido point to add to sceneBarre
    std::vector<QGraphicsEllipseItem*> ellipseVector;// holds Barrido points to paint in ellipse

    MainServer * server;// handle all kinect+server+client interactions
};
#endif // MAINWINDOW_H
