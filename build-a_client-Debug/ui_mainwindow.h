/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMenu2;
    QAction *actionOpcion2_1;
    QAction *actionOpcion1_2;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_8;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_server;
    QLineEdit *lineEdit;
    QSpacerItem *verticalSpacer;
    QPushButton *pbGo;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout;
    QLabel *labelVideo;
    QPushButton *pbVideo;
    QHBoxLayout *horizontalLayout_3;
    QLabel *labelDepth;
    QPushButton *pbDepth;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label3D;
    QPushButton *pb3D;
    QHBoxLayout *horizontalLayout_5;
    QLabel *label2D;
    QPushButton *pb2D;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelBarrido;
    QPushButton *pbBarrido;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labelAccel;
    QPushButton *pbAccel;
    QSpacerItem *verticalSpacer_3;
    QTextBrowser *textBrowser;
    QPushButton *pbStop;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    QGraphicsView *gvDepth;
    QGraphicsView *gvVideo;
    QGraphicsView *gvBarrido;
    FrameGL *glWidget;
    QWidget *tab_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(824, 596);
        actionMenu2 = new QAction(MainWindow);
        actionMenu2->setObjectName(QStringLiteral("actionMenu2"));
        actionOpcion2_1 = new QAction(MainWindow);
        actionOpcion2_1->setObjectName(QStringLiteral("actionOpcion2_1"));
        actionOpcion1_2 = new QAction(MainWindow);
        actionOpcion1_2->setObjectName(QStringLiteral("actionOpcion1_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_8 = new QHBoxLayout(centralWidget);
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setSizeConstraint(QLayout::SetFixedSize);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_server = new QLabel(centralWidget);
        label_server->setObjectName(QStringLiteral("label_server"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_server->sizePolicy().hasHeightForWidth());
        label_server->setSizePolicy(sizePolicy);
        label_server->setMinimumSize(QSize(50, 25));
        label_server->setMaximumSize(QSize(50, 25));

        horizontalLayout_2->addWidget(label_server);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMinimumSize(QSize(90, 25));
        lineEdit->setMaximumSize(QSize(90, 25));

        horizontalLayout_2->addWidget(lineEdit);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalSpacer = new QSpacerItem(85, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pbGo = new QPushButton(centralWidget);
        pbGo->setObjectName(QStringLiteral("pbGo"));
        pbGo->setEnabled(false);
        pbGo->setMinimumSize(QSize(80, 30));
        pbGo->setMaximumSize(QSize(80, 30));

        verticalLayout->addWidget(pbGo, 0, Qt::AlignHCenter);

        verticalSpacer_2 = new QSpacerItem(65, 28, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        labelVideo = new QLabel(centralWidget);
        labelVideo->setObjectName(QStringLiteral("labelVideo"));

        horizontalLayout->addWidget(labelVideo, 0, Qt::AlignHCenter);

        pbVideo = new QPushButton(centralWidget);
        pbVideo->setObjectName(QStringLiteral("pbVideo"));
        pbVideo->setMinimumSize(QSize(70, 30));

        horizontalLayout->addWidget(pbVideo);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        labelDepth = new QLabel(centralWidget);
        labelDepth->setObjectName(QStringLiteral("labelDepth"));

        horizontalLayout_3->addWidget(labelDepth, 0, Qt::AlignHCenter);

        pbDepth = new QPushButton(centralWidget);
        pbDepth->setObjectName(QStringLiteral("pbDepth"));
        pbDepth->setMinimumSize(QSize(70, 30));

        horizontalLayout_3->addWidget(pbDepth);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label3D = new QLabel(centralWidget);
        label3D->setObjectName(QStringLiteral("label3D"));

        horizontalLayout_4->addWidget(label3D, 0, Qt::AlignHCenter);

        pb3D = new QPushButton(centralWidget);
        pb3D->setObjectName(QStringLiteral("pb3D"));
        pb3D->setMinimumSize(QSize(70, 30));

        horizontalLayout_4->addWidget(pb3D);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        label2D = new QLabel(centralWidget);
        label2D->setObjectName(QStringLiteral("label2D"));

        horizontalLayout_5->addWidget(label2D, 0, Qt::AlignHCenter);

        pb2D = new QPushButton(centralWidget);
        pb2D->setObjectName(QStringLiteral("pb2D"));
        pb2D->setMinimumSize(QSize(70, 30));

        horizontalLayout_5->addWidget(pb2D);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        labelBarrido = new QLabel(centralWidget);
        labelBarrido->setObjectName(QStringLiteral("labelBarrido"));

        horizontalLayout_6->addWidget(labelBarrido, 0, Qt::AlignHCenter);

        pbBarrido = new QPushButton(centralWidget);
        pbBarrido->setObjectName(QStringLiteral("pbBarrido"));
        pbBarrido->setMinimumSize(QSize(70, 30));

        horizontalLayout_6->addWidget(pbBarrido);


        verticalLayout->addLayout(horizontalLayout_6);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        labelAccel = new QLabel(centralWidget);
        labelAccel->setObjectName(QStringLiteral("labelAccel"));

        horizontalLayout_7->addWidget(labelAccel, 0, Qt::AlignHCenter);

        pbAccel = new QPushButton(centralWidget);
        pbAccel->setObjectName(QStringLiteral("pbAccel"));
        pbAccel->setMinimumSize(QSize(70, 30));

        horizontalLayout_7->addWidget(pbAccel);


        verticalLayout->addLayout(horizontalLayout_7);

        verticalSpacer_3 = new QSpacerItem(108, 38, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer_3);

        textBrowser = new QTextBrowser(centralWidget);
        textBrowser->setObjectName(QStringLiteral("textBrowser"));
        textBrowser->setMinimumSize(QSize(140, 90));
        textBrowser->setMaximumSize(QSize(140, 16777215));

        verticalLayout->addWidget(textBrowser);

        pbStop = new QPushButton(centralWidget);
        pbStop->setObjectName(QStringLiteral("pbStop"));
        pbStop->setEnabled(false);
        pbStop->setMinimumSize(QSize(80, 30));
        pbStop->setMaximumSize(QSize(80, 30));

        verticalLayout->addWidget(pbStop, 0, Qt::AlignHCenter);


        horizontalLayout_8->addLayout(verticalLayout);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        gridLayout = new QGridLayout(tab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gvDepth = new QGraphicsView(tab);
        gvDepth->setObjectName(QStringLiteral("gvDepth"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(gvDepth->sizePolicy().hasHeightForWidth());
        gvDepth->setSizePolicy(sizePolicy1);
        gvDepth->setMinimumSize(QSize(320, 240));
        gvDepth->setBaseSize(QSize(320, 240));
        gvDepth->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvDepth->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(gvDepth, 0, 0, 1, 1);

        gvVideo = new QGraphicsView(tab);
        gvVideo->setObjectName(QStringLiteral("gvVideo"));
        sizePolicy1.setHeightForWidth(gvVideo->sizePolicy().hasHeightForWidth());
        gvVideo->setSizePolicy(sizePolicy1);
        gvVideo->setMinimumSize(QSize(320, 240));
        gvVideo->setBaseSize(QSize(320, 240));
        gvVideo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvVideo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(gvVideo, 0, 1, 1, 1);

        gvBarrido = new QGraphicsView(tab);
        gvBarrido->setObjectName(QStringLiteral("gvBarrido"));
        sizePolicy1.setHeightForWidth(gvBarrido->sizePolicy().hasHeightForWidth());
        gvBarrido->setSizePolicy(sizePolicy1);
        gvBarrido->setMinimumSize(QSize(320, 240));
        gvBarrido->setBaseSize(QSize(320, 240));

        gridLayout->addWidget(gvBarrido, 1, 0, 1, 1);

        glWidget = new FrameGL(tab);
        glWidget->setObjectName(QStringLiteral("glWidget"));
        glWidget->setMinimumSize(QSize(320, 240));
        glWidget->setBaseSize(QSize(320, 240));

        gridLayout->addWidget(glWidget, 1, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        horizontalLayout_8->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 824, 15));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionMenu2->setText(QApplication::translate("MainWindow", "opcion1_1", 0));
        actionOpcion2_1->setText(QApplication::translate("MainWindow", "opcion2_1", 0));
        actionOpcion1_2->setText(QApplication::translate("MainWindow", "opcion1_2", 0));
        label_server->setText(QApplication::translate("MainWindow", "server ip:", 0));
        pbGo->setText(QApplication::translate("MainWindow", "Go", 0));
        labelVideo->setText(QApplication::translate("MainWindow", "10005", 0));
        pbVideo->setText(QApplication::translate("MainWindow", "video", 0));
        labelDepth->setText(QApplication::translate("MainWindow", "10004", 0));
        pbDepth->setText(QApplication::translate("MainWindow", "depth", 0));
        label3D->setText(QApplication::translate("MainWindow", "10006", 0));
        pb3D->setText(QApplication::translate("MainWindow", "3D", 0));
        label2D->setText(QApplication::translate("MainWindow", "10007", 0));
        pb2D->setText(QApplication::translate("MainWindow", "2D", 0));
        labelBarrido->setText(QApplication::translate("MainWindow", "10008", 0));
        pbBarrido->setText(QApplication::translate("MainWindow", "barrido", 0));
        labelAccel->setText(QApplication::translate("MainWindow", "10009", 0));
        pbAccel->setText(QApplication::translate("MainWindow", "accel", 0));
        pbStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "views", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "data", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
