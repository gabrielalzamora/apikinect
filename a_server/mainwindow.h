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
#include "apikinect/typekinect.h"
#include "apikinect/maincore.h"

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

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void init();
    void setServerIp();
    void putKcombo();
    void initconnects();
    //gui interactions
    void apiconnects();
    void on_pbGo_clicked();
    void on_pbStop_clicked();
    void on_combo_activated(int index);
    //data link in tab_2
    void sliderVideoUp(int i);
    void sliderDepthUp(int i);
    void slider3DUp(int i);
    void sliderModuleUp(int i);
    //apikinect



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

    MainCore * apicore;//!< handle all kinect+server+client interactions
    int comboIndex;
};
#endif // MAINWINDOW_H
