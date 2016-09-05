/*
 * Copyright (c) 2016  Gabriel Alzamora.
 *
 * This code is licensed to you under the terms of the
 * GNU General Public License. See LICENSE file for the
 * text of the license, or the following URL:
 * https://www.gnu.org/licenses/gpl.html
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

/*!
 * \class MainWindow
 *
 */

/*!
 * \brief constructor
 *
 * MainWindow constructor starts sockets and graphics
 * objects to show data on gui and inherit ui class
 * ( in mainwindow.ui ) that contain buttons, menus,
 *  text edit... to interact.
 * ui->tab_2 is programmed in Data class
 * \param parent
 * not used
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectedServer = 0;
    skt_srvK = new QTcpSocket(this);
    //video
    skt_video = new QTcpSocket(this);
    connectedVideo = 0;
    sizeVideo = 0;
    sceneVideo = new QGraphicsScene(this);
    ui->gvVideo->setScene(sceneVideo);
    //depth
    skt_depth = new QTcpSocket(this);
    connectedDepth = 0;
    sizeDepth = 0;
    sceneDepth = new QGraphicsScene(this);
    ui->gvDepth->setScene(sceneDepth);
    //3D
    skt_3D = new QTcpSocket(this);
    connected3D = 0;
    size3D = 0;
    p3Buf.reserve(300000);
    p3Buf.resize(0);
    //2D
    skt_2D = new QTcpSocket(this);
    connected2D = 0;
    size2D = 0;
    p2Buf.reserve(300000);
    p2Buf.resize(0);
    //barrido
    skt_barrido = new QTcpSocket(this);
    connectedBarrido = 0;
    sizeBarrido = 0;
    sceneBarrido = new QGraphicsScene(this);
    ui->gvBarrido->setScene(sceneBarrido);
    p3Buf.resize(300000);//reserve for maximum number of points
    barridoBuf.resize(360);
    ellipseVector.reserve(360);
    ellipseVector.resize(0);
    ellipse = NULL;
    //accel
    skt_accel = new QTcpSocket(this);
    connectedAccel = 0;
    sizeAccel = 0;
    acceleration.accel_x = acceleration.accel_y = acceleration.accel_z = 0.0;

    connect(ui->lineEdit,SIGNAL(editingFinished()),this,SLOT(setHost()));

    connect(ui->pbGo,SIGNAL(released()),this,SLOT(initConnection()));
    connect(ui->pbStop,SIGNAL(released()),this,SLOT(closeConnection()));

    connect(ui->tab_2,SIGNAL(dataChanged()),this,SLOT(dataChanged()));
    connect(ui->tab_2,SIGNAL(ledOptionChanged()),this,SLOT(dataChanged()));

    connect(ui->pbVideo,SIGNAL(released()),this,SLOT(initVideo()));
    connect(skt_video,SIGNAL(readyRead()),this,SLOT(readDataVideo()));

    connect(ui->pbDepth,SIGNAL(released()),this,SLOT(initDepth()));
    connect(skt_depth,SIGNAL(readyRead()),this,SLOT(readDataDepth()));

    connect(ui->pb3D,SIGNAL(released()),this,SLOT(init3D()));
    connect(skt_3D,SIGNAL(readyRead()),this,SLOT(readData3D()));

    connect(ui->pb2D,SIGNAL(released()),this,SLOT(init2D()));
    connect(skt_2D,SIGNAL(readyRead()),this,SLOT(readData2D()));

    connect(ui->pbBarrido,SIGNAL(released()),this,SLOT(initBarrido()));
    connect(skt_barrido,SIGNAL(readyRead()),this,SLOT(readDataBarrido()));

    connect(ui->pbAccel,SIGNAL(released()),this,SLOT(initAccel()));
    connect(skt_accel,SIGNAL(readyRead()),this,SLOT(readDataAccel()));

    barridoAxes();
}
/*!
 * \brief destructor
 * to free memory and close properly
 */
MainWindow::~MainWindow()
{
    delete ui;
    delete skt_srvK;
    delete skt_video;
    delete skt_depth;
    //...
}
/*!
 * \brief auxiliary function to read host written by user in GUI QLineEdit
 */
void MainWindow::setHost()
{
//    ui->lineEdit->setText("192.168.1.34");///-----------------DEBUG
//    ui->lineEdit->setText("192.168.0.157");///-----------------DEBUG
//    ui->lineEdit->setText("127.0.0.1");///-----------------DEBUG
    hostAddr = QHostAddress(ui->lineEdit->text());
    ui->textBrowser->setText("connect to server ip:");
    ui->textBrowser->append(ui->lineEdit->text());
    ui->pbGo->setEnabled(true);
}
/*!
 * \brief send new srvKinect to server
 */
void MainWindow::dataChanged()
{
    //qDebug("MainClient::dataChanged");
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out << ui->tab_2->m_srvK.m_fAngulo;
    out << ui->tab_2->m_srvK.m_iAnguloKinect;
    out << ui->tab_2->m_srvK.m_fAltura;
    out << ui->tab_2->m_srvK.m_fYMin;
    out << ui->tab_2->m_srvK.m_fYMax;
    out << ui->tab_2->m_srvK.m_fZMax;
    out << ui->tab_2->m_srvK.m_ulRefresco3D;
    out << ui->tab_2->m_srvK.m_usModulo3D;
    out << ui->tab_2->m_srvK.m_bEnvio3D;
    out << ui->tab_2->m_srvK.m_bEnvio2D;
    out << ui->tab_2->m_srvK.m_bEnvioBarrido;
    out << ui->tab_2->m_srvK.m_bCompress3D;
    out << ui->tab_2->m_srvK.m_iBarridoEcu;
    out << ui->tab_2->m_srvK.m_iBarridoYMin;
    out << ui->tab_2->m_srvK.m_iBarridoYMax;
    out << ui->tab_2->m_srvK.m_ulRefrescoDepth;
    out << ui->tab_2->m_srvK.m_bEnvioDepth;
    out << ui->tab_2->m_srvK.m_bCompressDepth;
    out << ui->tab_2->m_srvK.m_ulRefrescoColor;
    out << ui->tab_2->m_srvK.m_bEnvioColor;
    out << ui->tab_2->m_srvK.m_bCompressColor;
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_srvK->write(buff);
    //qDebug("  init tamaño enviado: %u", (buff.size()-sizeof(quint64)));//DEBUG
}
/*!
 * \brief utility function to request new data to socket (in Apikinect way)
 * \param [in] socket to which request new info.
 */
void MainWindow::requestNext(QTcpSocket *socket)
{
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    socket->write(buff);
}
/*!
 * \brief utility function to request disconnect to server socket (in Apikinect way)
 * \param [in] socket to which request new info.
 */
void MainWindow::requestStop(QTcpSocket *socket)
{
    QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    socket->write(buff);
}
/*!
 * \brief Connect to server and srvKinect update comm.
 */
void MainWindow::initConnection()
{
    //qDebug("MainClient::initConnection");
    QString msg;
    msg = "Unable to connect to server\ncheck server ip\n";

    skt_srvK->connectToHost(hostAddr,SRVKPORT);
    if( !skt_srvK->waitForConnected(3000) ){
        msg.append(skt_srvK->errorString());
        ui->textBrowser->setText(msg);
        qDebug("  client NOT connected connectedServer = %d",connectedServer);
        return;
    }
    ui->pbStop->setEnabled(true);//let's activate stop button
    ui->pbGo->setEnabled(false);
    connectedServer = 1;
    ui->textBrowser->append("  client connected to server");
    dataChanged();
}
/*!
 * \brief disconnect from server all sockets & close
 */
void MainWindow::closeConnection()
{
    qDebug("MainClient::closeConnection");
    skt_srvK->disconnectFromHost();
    connectedServer = 0;
    if(connectedVideo){
        finalizeVideo();
    }
    if(connectedDepth){
        finalizeDepth();
    }
    ui->pbGo->setEnabled(true);
    ui->pbStop->setEnabled(false);
}

void MainWindow::socketErrorVideo()
{

}

/*!
 * \brief start/stop video connection to server
 *
 * clicked twice stops video
 */
void MainWindow::initVideo()
{
    //qDebug("MainClient::initVideo");
    if( connectedVideo ){//second click disconnects
        finalizeVideo();
        ui->textBrowser->append("- video closed ");
        return;
    }else{
        skt_video->connectToHost(hostAddr,VIDEOPORT);
        if( !skt_video->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_video->errorString());
            qDebug("  client VIDEO NOT connected = %d",connectedVideo);
            return;
        }
        //connect(skt_video,SIGNAL(readyRead()),this,SLOT(readDataVideo()));
        connectedVideo = 1;
        ui->textBrowser->append("- video connected");
        //qDebug("  connectedVideo = %i",connectedVideo);
    }
    sizeVideo = 0;
    requestNext(skt_video);//request next video image to server
}
/*!
 * \brief stop video connection to server
 */
void MainWindow::finalizeVideo()
{
    //qDebug("MainClient::finalizeVideo");
    //qDebug("  connectedVideo = %i",connectedVideo);

    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);//tell 0 to server = stop
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    //qDebug("  tamaño video antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    skt_video->write(buff);//send 0 to server
    */
    requestStop(skt_video);//request STOP video to server
    skt_video->disconnectFromHost();
    connectedVideo = 0;
    //qDebug("  connectedVideo = %i",connectedVideo);
    //qDebug() << buff;
}
/*!
 * \brief read video data and show on gui
 */
void MainWindow::readDataVideo()
{
    //qDebug("MainClient::readDataVideo");
    QDataStream ioStream(skt_video);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeVideo == 0){
        if(skt_video->bytesAvailable() < (int)sizeof(quint64))
            return;//wait till sizeVideo completed-----or-------return
        ioStream >> sizeVideo;
    }
    if(skt_video->bytesAvailable() < (sizeVideo-sizeof(quint64)))
        return;//wait till all image data received

    QImage image;
    ioStream >> image;

    //-----------------------------------show image
    sceneVideo->addPixmap(QPixmap::fromImage(image).scaled(ui->gvDepth->width()-2,ui->gvDepth->height()-2,Qt::KeepAspectRatio));
    //sceneVideo->addPixmap(QPixmap::fromImage(image).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvVideo->show();

    sizeVideo = 0;//to allow get next img sizeVideo
    requestNext(skt_video);//request to server new video frame
}

/*!
 * \brief start/stop depth connection to server
 *
 * clicked twice stops depth
 */
void MainWindow::initDepth()
{
    //qDebug("MainClient::initDepth");
    if( connectedDepth ){//second click disconnects
        finalizeDepth();
        ui->textBrowser->append("- depth closed ");
        return;
    }else{
        skt_depth->connectToHost(hostAddr,DEPTHPORT);
        if( !skt_depth->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_depth->errorString());
            qDebug("  client DEPTH NOT connected = %d",connectedDepth);
            return;
        }
        connectedDepth = 1;
        ui->textBrowser->append("- depth connected");
    }
    //---------------------------request 1st image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_depth->write(buff);
    //qDebug("  tamaño depth antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    sizeDepth = 0;
    requestNext(skt_depth);
}
/*!
 * \brief stop depth connection to server
 */
void MainWindow::finalizeDepth()
{
    //qDebug("MainClient::finalizeDepth");
    //---------------------------request STOP server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);//tell 0 to server = stop
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_depth->write(buff);//send 0 to server
    //qDebug("  tamaño depth antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    requestStop(skt_depth);
    skt_depth->disconnectFromHost();
    connectedDepth = 0;
}
/*!
 * \brief read depth data and show on gui
 */
void MainWindow::readDataDepth()
{
   // qDebug("MainClient::readDataDepth");
    QDataStream ioStream(skt_depth);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeDepth == 0){
        if(skt_depth->bytesAvailable() < (int)sizeof(quint64))
            return;//wait till sizeDepth completed-----or-------return
        ioStream >> sizeDepth;

    }
    if(skt_depth->bytesAvailable() < (sizeDepth-sizeof(quint64)))
        return;//wait till all image data received

    QImage image;
    ioStream >> image;

    //-----------------------------------show depth
    sceneDepth->addPixmap(QPixmap::fromImage(image).scaled(ui->gvDepth->width()-2,ui->gvDepth->height()-2,Qt::KeepAspectRatio));
    //sceneDepth->addPixmap(QPixmap::fromImage(image).scaled(320,240,Qt::KeepAspectRatio));
    ui->gvDepth->show();

    //---------------------------request next image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//escribimos tamaño buf al principio
    skt_depth->write(buff);//enviamos
    */
    sizeDepth = 0;//to allow get next img sizeDepth
    requestNext(skt_depth);
}

/*!
 * \brief start/stop 3D connection to server
 *
 * clicked twice stops 3d
 */
void MainWindow::init3D()
{
    //qDebug("MainClient::init3D");
    if( connected3D ){//second click disconnects
        finalize3D();
        ui->textBrowser->append("- 3D closed ");
        return;
    }else{
        skt_3D->connectToHost(hostAddr,THREEPORT);
        if( !skt_3D->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_3D->errorString());
            qDebug("  client 3D NOT connected = %d",connected3D);
            return;
        }
        //connect(skt_3D,SIGNAL(readyRead()),this,SLOT(readData3D()));
        connected3D = 1;
        ui->textBrowser->append("- 3D connected");
    }
    //---------------------------request 1st image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_3D->write(buff);
    qDebug("  tamaño 3D antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    size3D = 0;
    requestNext(skt_3D);
}
/*!
 * \brief stop 3D connection to server
 */
void MainWindow::finalize3D()
{
    //qDebug("MainClient::finalize3D");
    //---------------------------request STOP server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);//tell 0 to server = stop
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_3D->write(buff);//send 0 to server
    //qDebug("  tamaño depth antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    requestStop(skt_3D);
    skt_3D->disconnectFromHost();
    connected3D = 0;
}
/*!
 * \brief read 3d data and show on gui
 */
void MainWindow::readData3D()
{
    //qDebug("MainClient::readData3D");
    QDataStream ioStream(skt_3D);
    ioStream.setVersion(QDataStream::Qt_5_0);
    p3Buf.resize(0);//still has memory allocated (reserved), but size = 0
    point3c aux3;//remember aux3 = x, y, z & color(red, green, blue & reserved)

    if(size3D == 0){
        if(skt_3D->bytesAvailable() < (int)sizeof(quint64))
            return;//size3D info completed-----or-------return
        ioStream >> size3D;
        //qDebug("  tamaño recibido %llu",size3D);
    }
    if(skt_3D->bytesAvailable() < (size3D-sizeof(quint64)))
        return;//wait till all 3D data received

//-----------------------------------read & paint 3D
    for(int i=0;i<(size3D/sizeof(point3c));i++){
        ioStream >> aux3.x;//read one point3c x dimension
        ioStream >> aux3.y;
        ioStream >> aux3.z;
        ioStream >> aux3.color.rgbRed;//read point3c color red component
        ioStream >> aux3.color.rgbGreen;
        ioStream >> aux3.color.rgbBlue;
        ioStream >> aux3.color.rgbReserved;
        p3Buf.push_back(aux3);//store in p3Buf (3D vector)
    }
    //qDebug("  tamaño vector %lu",p3Buf.size());
    ui->glWidget->setCloud(p3Buf);
    ui->glWidget->repaint();

    //---------------------------request next image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//escribimos tamaño buf al principio
    skt_3D->write(buff);//enviamos
    */
    size3D = 0;//to allow get next point cloud size3D
    requestNext(skt_3D);
}

/*!
 * \brief start/stop 2D connection to server
 *
 * clicked twice stops 2d
 */
void MainWindow::init2D()
{
    //qDebug("MainClient::init2D");
    if( connected2D ){//second click disconnects
        finalize2D();
        ui->textBrowser->append("- 2D closed ");
        return;
    }else{
        skt_2D->connectToHost(hostAddr,TWOPORT);
        if( !skt_2D->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_2D->errorString());
            qDebug("  client 2D NOT connected = %d",connected2D);
            return;
        }
        //connect(skt_2D,SIGNAL(readyRead()),this,SLOT(readData2D()));
        connected2D = 1;
        ui->textBrowser->append("- 2D connected");
    }
    //---------------------------request 1st image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_2D->write(buff);
    qDebug("  tamaño 2D antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    size2D = 0;
    requestNext(skt_2D);
}
/*!
 * \brief stop 2D connection to server
 */
void MainWindow::finalize2D()
{
    //qDebug("MainClient::finalize2D");
    //---------------------------request STOP server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);//tell 0 to server = stop
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_2D->write(buff);//send 0 to server
    //qDebug("  tamaño depth antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    requestStop(skt_2D);
    skt_2D->disconnectFromHost();
    connected2D = 0;
}
/*!
 * \brief read 2d data and show on gui
 */
void MainWindow::readData2D()
{
    //qDebug("MainClient::readData2D");
    QDataStream ioStream(skt_2D);
    ioStream.setVersion(QDataStream::Qt_5_0);
    p2Buf.resize(0);//still has memory allocated (reserved), but size = 0
    point2 aux2;//remember aux2 = x & z

    if(size2D == 0){
        if(skt_2D->bytesAvailable() < (int)sizeof(quint64))
            return;//size2D info completed-----or-------return
        ioStream >> size2D;
        //qDebug("  tamaño recibido %llu",size2D);
    }
    if(skt_2D->bytesAvailable() < (size2D-sizeof(quint64)))
        return;//wait till all 2D data received

//-----------------------------------read & paint 2D
    for(int i=0;i<(size2D/sizeof(point2));i++){
        ioStream >> aux2.x;//read one point2c x dimension
        ioStream >> aux2.z;
        p2Buf.push_back(aux2);//store in p2Buf (2D vector)
    }
    //qDebug("  tamaño vector %lu",p2Buf.size());
    ui->glWidget->setCloud(p2Buf);
    ui->glWidget->repaint();

    //---------------------------request next image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//escribimos tamaño buf al principio
    skt_2D->write(buff);//enviamos
    */

    size2D = 0;//to allow get next point cloud size2D
    requestNext(skt_2D);
}

/*!
 * \brief MainWindow::barreInit
 * draw axes on sceneBarre to show on gvBarrido.
 */
void MainWindow::barridoAxes()
{
    ui->gvBarrido->setBackgroundBrush(QColor(200,240,240,255));//light blue
    sceneBarrido->setBackgroundBrush(QColor(200,240,240,255));
    QPen ejesPen = QPen(QColor(255,0,0,255));//red color
    ejesPen.setWidth(2);
    QLine ejex = QLine(5,230,315,230);
    QLine ejey = QLine(160,230,160,5);
    //falta pintar escala cada ~500mm
    sceneBarrido->addLine(ejex,ejesPen);
    sceneBarrido->addLine(ejey,ejesPen);
}
/*!
 * \brief start/stop barrido connection to server
 *
 * clicked twice stops barrido
 */
void MainWindow::initBarrido()
{
    //qDebug("MainClient::initBarrido");
    if( connectedBarrido ){//second click disconnects
        finalizeBarrido();
        ui->textBrowser->append("- barrido closed ");
        return;
    }else{
        skt_barrido->connectToHost(hostAddr,BARRIDOPORT);
        if( !skt_barrido->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_barrido->errorString());
            qDebug("  client Barrido NOT connected = %d",connectedBarrido);
            return;
        }
        //connect(skt_barrido,SIGNAL(readyRead()),this,SLOT(readDataBarrido()));
        connectedBarrido = 1;
        ui->textBrowser->append("- barrido connected");
    }
    //---------------------------request 1st data batch to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    skt_barrido->write(buff);
    qDebug("  tamaño antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    sizeBarrido = 0;
    requestNext(skt_barrido);
}
/*!
 * \brief stop barrido connection to server
 */
void MainWindow::finalizeBarrido()
{
    //qDebug("MainClient::finalizeBarrido");
    //---------------------------tell STOP to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(0);//tell server 0, stop
    out.device()->seek(0);
    out << quint64(buff.size() - sizeof(quint64));
    qDebug("  tamaño antes enviado: %lu", (buff.size()-sizeof(quint64)));///---DEBUG
    skt_barrido->write(buff);//send 0 to server
    //qDebug("  tamaño barrido antes enviado: %lu", (buff.size()-sizeof(quint64)));//DEBUG
    */
    requestStop(skt_barrido);
    skt_barrido->disconnectFromHost();
    connectedBarrido = 0;
}
/*!
 * \brief read barrido data and show on gui
 */
void MainWindow::readDataBarrido()
{
    //qDebug("MainClient::readDataBarrido");
    QDataStream in(skt_barrido);
    in.setVersion(QDataStream::Qt_5_0);///---------DEBUG

    if(sizeBarrido == 0){
        if(skt_barrido->bytesAvailable() < (int)sizeof(quint64))
            return;//wait till sizeBarrido completed-----or-------return
        in >> sizeBarrido;
        //qDebug("  barrido recibe tamaño %llu",sizeBarrido);
    }
    if(skt_barrido->bytesAvailable() < (sizeBarrido-sizeof(quint64)))
        return;//wait till all data received

//-----------------------------------------------------read & paint barrido
    int x, y, aux(0);
    uint32_t auxBarrido;
    if( ellipseVector.size() != 0 ){//remove previous data
        for(int i=0;i<ellipseVector.size();i++){
            sceneBarrido->removeItem(ellipseVector[i]);
            delete ellipseVector[i];
        }
        ellipseVector.resize(0);
    }
    for(int i=0;i<360;i++){
        in >> auxBarrido;//read incomming data
        barridoBuf[i] = auxBarrido;//store in barrido vector
        //qDebug("  compara read %u con vector %u", auxBarrido, barridoBuf[i]);
        if(barridoBuf[i] == 0) continue;//no data info no plot
        y = (H/2)-((H/2)*barridoBuf[i]/ui->tab_2->m_srvK.m_fZMax);//divide barridoBuf[i] by max distance to fit in screen
        x = (W/2)*(360-i)/360;//make it fit in window W*H
        ellipseVector.push_back(new QGraphicsEllipseItem(x,y,1.0,1.0));
        sceneBarrido->addItem(ellipseVector[aux]);
        aux++;
    }
    ui->gvBarrido->show();
//-------------------------------------------------------paint barrido
    //---------------------------request next image to server
    /*QByteArray buff;
    QDataStream out(&buff, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_0);
    out << quint64(0) << quint8(1);
    out.device()->seek(0);//puntero a inicio buff
    out << quint64(buff.size() - sizeof(quint64));//escribimos tamaño buf al principio
    skt_barrido->write(buff);//enviamos
    //qDebug("  barrido envia tamaño %lu",(buff.size() - sizeof(quint64)));
    */
    sizeBarrido = 0;//to allow get next img sizeBarrido
    requestNext(skt_barrido);
}

void MainWindow::initAccel()
{
    //qDebug("MainClient::initAccel");
    if( connectedAccel ){//second click disconnects
        finalizeAccel();
        ui->textBrowser->append("- accel closed ");
        return;
    }else{
        skt_accel->connectToHost(hostAddr,ACCELPORT);
        if( !skt_accel->waitForConnected(3000) ){
            ui->textBrowser->setText(skt_accel->errorString());
            qDebug("  client Accel NOT connected = %d",connectedAccel);
            return;
        }
        //connect(skt_accel,SIGNAL(readyRead()),this,SLOT(readDataAccel()));
        connectedAccel = 1;
        ui->textBrowser->append("- accel connected");
    }
    sizeAccel = 0;
    requestNext(skt_accel);
}

void MainWindow::finalizeAccel()
{
    //qDebug("MainClient::finalizeAccel");
    requestStop(skt_accel);
    skt_accel->disconnectFromHost();
    connectedAccel = 0;
}

void MainWindow::readDataAccel()
{
    //qDebug("MainClient::readDataAccel");
    QDataStream ioStream(skt_accel);
    ioStream.setVersion(QDataStream::Qt_5_0);

    if(sizeAccel == 0){
        if(skt_accel->bytesAvailable() < (int)sizeof(quint64))
            return;//size2D info completed-----or-------return
        ioStream >> sizeAccel;
        //qDebug("  tamaño recibido %llu",sizeAccel);
    }
    if(skt_accel->bytesAvailable() < (sizeAccel-sizeof(quint64)))
        return;//wait till all 2D data received

//-----------------------------------read & paint 2D
    ioStream >> acceleration.accel_x;
    ioStream >> acceleration.accel_y;
    ioStream >> acceleration.accel_z;

    showAccel(acceleration);

    sizeAccel = 0;
    requestNext(skt_accel);
}

void MainWindow::showAccel(accel a)
{
    QString str = "aX = ";
    QString aux;
    aux.setNum(a.accel_x);
    str.append(aux);
    str.append("\n");
    aux = "aY = ";
    str.append(aux);
    aux.setNum(a.accel_y);
    str.append(aux);
    str.append("\n");
    aux = "aZ = ";
    str.append(aux);
    aux.setNum(a.accel_z);
    str.append(aux);
    str.append("\n");
    ui->textBrowser->setText(str);
}
