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
    void ledChanged(freenect_led_options lop);
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
    void updateSrvKinect(srvKinect newSrvK);
    void setSrvKinect(srvKinect newSrvK);//->ConfigData
    srvKinect getSrvKinect();//<-ConfigData

protected:
    void closeEvent(QCloseEvent *event) Q_DECL_OVERRIDE;

private slots:
    void init();
    void setServerIp();
    void putKcombo();
    void initconnects();

    void on_pbGo_clicked();
    void on_pbStop_clicked();
    void on_combo_highlighted(int index);
    //data link in tab_2
    void sliderVideoUp(QString s);
    void sliderDepthUp(QString s);
    void slider3DUp(QString s);
    void sliderModuleUp(QString s);

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

    MainCore * apicore;
};
#endif // MAINWINDOW_H
