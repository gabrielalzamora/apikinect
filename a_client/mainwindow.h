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
#include <QHostAddress>
//#include "apikinect/configdata.h"
#include "apikinect/mainclient.h"

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
    void srvKinectChanged(srvKinect newSrvK);//GUI->client->server new srvKinect
    void ledChanged(int lop);
    void irChanged(int i);
    //void go(int kindex);//server
    //void stop(int kindex);//server

public slots:
    void paintVideo();
    void paintDepth();
    void paintBarrido();
    void paintBarridoAxes();
    void paint3D();
    void paint2D();
    void paintTimeVector();
    void setSrvKinect(srvKinect newSrvK);//ConfigData => ui->tab_2
    srvKinect getSrvKinect();//ui->tab_2 => ConfigData
    void printMessage(QString str);

private slots:
    void setHost();
    void init();
    void initConnects();
    //gui interactions
    void apiConnects();
    void on_pbGo_clicked();
    void on_pbDepth_clicked();
    void on_pbVideo_clicked();
    void on_pb3D_clicked();
    void on_pb2D_clicked();
    void on_pbBarrido_clicked();
    void on_pbAccel_clicked();

    void on_lineEdit_editingFinished();
    //data link in tab_2
    void sliderVideoUp(int i);
    void sliderDepthUp(int i);
    void slider3DUp(int i);
    void sliderModuleUp(int i);
    void upServerSrvK(int i);

private:
    Ui::MainWindow *ui;
    QHostAddress hostAddr;
    QGraphicsScene *sceneVideo;
    QGraphicsScene *sceneDepth;
    QGraphicsScene *sceneBarre;
    QImage *imgVideo;
    QImage *imgDepth;
    QImage *imgBarre;
    QGraphicsEllipseItem *ellipse;//!< holds single Barrido point to add to sceneBarrido
    std::vector<QGraphicsEllipseItem*> ellipseVector;//!< holds barrido points to paint in ellipse

    MainClient * client;// handle all client+server interactions

    void showK(srvKinect srvK);///------------------------------------------------------DEBUG
};

#endif // MAINWINDOW_H
