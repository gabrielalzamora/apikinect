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
#include <QTimer>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QTcpSocket>
#include <QHostAddress>
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

public slots:
    void setHost();
    void dataChanged();

private slots:
    void requestNext(QTcpSocket *socket);
    void requestStop(QTcpSocket *socket);
    //server
    void initConnection();
    void closeConnection();
    void socketErrorVideo();
    //video
    void initVideo();
    void finalizeVideo();
    void readDataVideo();
    //depth
    void initDepth();
    void finalizeDepth();
    void readDataDepth();
    //3D
    void init3D();
    void finalize3D();
    void readData3D();
    //2D
    void init2D();
    void finalize2D();
    void readData2D();
    //barrido
    void barridoAxes();
    void initBarrido();
    void finalizeBarrido();
    void readDataBarrido();
    //accel
    void initAccel();
    void finalizeAccel();
    void readDataAccel();
    void showAccel(accel a);

private:
    Ui::MainWindow *ui;
    QHostAddress hostAddr;//------------------DEBUG
    QTcpSocket *skt_srvK;
    int connectedServer;
    //Video
    QTcpSocket *skt_video;
    int connectedVideo;
    quint64 sizeVideo;
    QGraphicsScene *sceneVideo;
    QGraphicsItem *itemVideo;
    //Depth
    QTcpSocket *skt_depth;
    int connectedDepth;
    quint64 sizeDepth;
    QGraphicsScene *sceneDepth;
    QGraphicsItem *itemDepth;
    //3D
    QTcpSocket *skt_3D;
    int connected3D;
    quint64 size3D;
    std::vector<point3c> p3Buf;
    //2D
    QTcpSocket *skt_2D;
    int connected2D;
    quint64 size2D;
    std::vector<point2>p2Buf;
    //Barrido
    QTcpSocket *skt_barrido;
    int connectedBarrido;
    quint64 sizeBarrido;
    QGraphicsScene *sceneBarrido;
    QGraphicsEllipseItem *ellipse;//!< holds single Barrido point to add to sceneBarrido
    std::vector<QGraphicsEllipseItem*> ellipseVector;//!< holds barrido points to paint in ellipse
    std::vector<uint32_t> barridoBuf;
    //Accel
    QTcpSocket *skt_accel;
    int connectedAccel;
    quint64 sizeAccel;
    accel acceleration;

};

#endif // MAINWINDOW_H
