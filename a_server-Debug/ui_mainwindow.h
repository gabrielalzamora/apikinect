/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include <data.h>
#include <framegl.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMenu2;
    QAction *actionOpcion2_1;
    QAction *actionOpcion1_2;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QSpacerItem *verticalSpacer_2;
    QSpacerItem *verticalSpacer_5;
    QSpacerItem *verticalSpacer;
    QPushButton *pbStop;
    QSpacerItem *verticalSpacer_4;
    QComboBox *combo;
    QCheckBox *cb_video;
    QSpacerItem *verticalSpacer_3;
    QLabel *label_k;
    QCheckBox *cb_3;
    QLabel *label_ip;
    QTextEdit *textEdit;
    QCheckBox *cb_depth;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    FrameGL *glWidget;
    QGraphicsView *gvDepth;
    QGraphicsView *gvBarrido;
    QGraphicsView *gvVideo;
    Data *tab_2;
    QCheckBox *cb_2;
    QLabel *label_server;
    QCheckBox *cb_barrido;
    QPushButton *pbGo;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(817, 610);
        actionMenu2 = new QAction(MainWindow);
        actionMenu2->setObjectName(QStringLiteral("actionMenu2"));
        actionOpcion2_1 = new QAction(MainWindow);
        actionOpcion2_1->setObjectName(QStringLiteral("actionOpcion2_1"));
        actionOpcion1_2 = new QAction(MainWindow);
        actionOpcion1_2->setObjectName(QStringLiteral("actionOpcion1_2"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        verticalSpacer_2 = new QSpacerItem(65, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 5, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(58, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 10, 0, 1, 1);

        verticalSpacer = new QSpacerItem(85, 10, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding);

        gridLayout_2->addItem(verticalSpacer, 2, 0, 1, 1);

        pbStop = new QPushButton(centralWidget);
        pbStop->setObjectName(QStringLiteral("pbStop"));
        pbStop->setEnabled(false);
        QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pbStop->sizePolicy().hasHeightForWidth());
        pbStop->setSizePolicy(sizePolicy);
        pbStop->setMinimumSize(QSize(80, 30));
        pbStop->setMaximumSize(QSize(80, 30));

        gridLayout_2->addWidget(pbStop, 9, 0, 1, 1, Qt::AlignHCenter);

        verticalSpacer_4 = new QSpacerItem(28, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 16, 0, 1, 1);

        combo = new QComboBox(centralWidget);
        combo->setObjectName(QStringLiteral("combo"));
        combo->setMinimumSize(QSize(80, 20));
        combo->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(combo, 4, 0, 1, 1);

        cb_video = new QCheckBox(centralWidget);
        cb_video->setObjectName(QStringLiteral("cb_video"));
        cb_video->setChecked(true);

        gridLayout_2->addWidget(cb_video, 12, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(88, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 8, 0, 1, 1);

        label_k = new QLabel(centralWidget);
        label_k->setObjectName(QStringLiteral("label_k"));
        label_k->setMinimumSize(QSize(80, 0));
        label_k->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(label_k, 3, 0, 1, 1);

        cb_3 = new QCheckBox(centralWidget);
        cb_3->setObjectName(QStringLiteral("cb_3"));

        gridLayout_2->addWidget(cb_3, 14, 0, 1, 1);

        label_ip = new QLabel(centralWidget);
        label_ip->setObjectName(QStringLiteral("label_ip"));
        label_ip->setMinimumSize(QSize(80, 0));
        label_ip->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(label_ip, 1, 0, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(textEdit, 17, 0, 1, 1);

        cb_depth = new QCheckBox(centralWidget);
        cb_depth->setObjectName(QStringLiteral("cb_depth"));
        cb_depth->setChecked(true);

        gridLayout_2->addWidget(cb_depth, 11, 0, 1, 1);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(670, 530));
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        QSizePolicy sizePolicy1(QSizePolicy::Expanding, QSizePolicy::Expanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(tab->sizePolicy().hasHeightForWidth());
        tab->setSizePolicy(sizePolicy1);
        tab->setMinimumSize(QSize(666, 500));
        tab->setSizeIncrement(QSize(1, 1));
        gridLayout = new QGridLayout(tab);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        glWidget = new FrameGL(tab);
        glWidget->setObjectName(QStringLiteral("glWidget"));
        glWidget->setMinimumSize(QSize(320, 240));
        glWidget->setBaseSize(QSize(320, 240));

        gridLayout->addWidget(glWidget, 1, 1, 1, 1);

        gvDepth = new QGraphicsView(tab);
        gvDepth->setObjectName(QStringLiteral("gvDepth"));
        gvDepth->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvDepth->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(gvDepth, 0, 0, 1, 1);

        gvBarrido = new QGraphicsView(tab);
        gvBarrido->setObjectName(QStringLiteral("gvBarrido"));

        gridLayout->addWidget(gvBarrido, 1, 0, 1, 1);

        gvVideo = new QGraphicsView(tab);
        gvVideo->setObjectName(QStringLiteral("gvVideo"));
        gvVideo->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        gvVideo->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        gridLayout->addWidget(gvVideo, 0, 1, 1, 1);

        tabWidget->addTab(tab, QString());
        tab_2 = new Data();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        tabWidget->addTab(tab_2, QString());

        gridLayout_2->addWidget(tabWidget, 0, 1, 18, 1);

        cb_2 = new QCheckBox(centralWidget);
        cb_2->setObjectName(QStringLiteral("cb_2"));

        gridLayout_2->addWidget(cb_2, 15, 0, 1, 1);

        label_server = new QLabel(centralWidget);
        label_server->setObjectName(QStringLiteral("label_server"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_server->sizePolicy().hasHeightForWidth());
        label_server->setSizePolicy(sizePolicy2);
        label_server->setMinimumSize(QSize(80, 0));
        label_server->setMaximumSize(QSize(130, 30));

        gridLayout_2->addWidget(label_server, 0, 0, 1, 1);

        cb_barrido = new QCheckBox(centralWidget);
        cb_barrido->setObjectName(QStringLiteral("cb_barrido"));
        cb_barrido->setChecked(false);

        gridLayout_2->addWidget(cb_barrido, 13, 0, 1, 1);

        pbGo = new QPushButton(centralWidget);
        pbGo->setObjectName(QStringLiteral("pbGo"));
        pbGo->setEnabled(false);
        sizePolicy.setHeightForWidth(pbGo->sizePolicy().hasHeightForWidth());
        pbGo->setSizePolicy(sizePolicy);
        pbGo->setMinimumSize(QSize(80, 30));
        pbGo->setMaximumSize(QSize(80, 30));

        gridLayout_2->addWidget(pbGo, 7, 0, 1, 1, Qt::AlignHCenter);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 817, 22));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        actionMenu2->setText(QApplication::translate("MainWindow", "opcion1_1", 0));
        actionOpcion2_1->setText(QApplication::translate("MainWindow", "opcion2_1", 0));
        actionOpcion1_2->setText(QApplication::translate("MainWindow", "opcion1_2", 0));
        pbStop->setText(QApplication::translate("MainWindow", "Stop", 0));
        combo->setCurrentText(QString());
        cb_video->setText(QApplication::translate("MainWindow", "video shown", 0));
        label_k->setText(QApplication::translate("MainWindow", "select k", 0));
        cb_3->setText(QApplication::translate("MainWindow", "3d shown", 0));
        label_ip->setText(QApplication::translate("MainWindow", "localhost", 0));
        cb_depth->setText(QApplication::translate("MainWindow", "depth shown", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "views", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "data", 0));
        cb_2->setText(QApplication::translate("MainWindow", "2d shown", 0));
        label_server->setText(QApplication::translate("MainWindow", "server ip:", 0));
        cb_barrido->setText(QApplication::translate("MainWindow", "barrrido shown", 0));
        pbGo->setText(QApplication::translate("MainWindow", "Go", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
