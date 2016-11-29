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
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <apikinect/framegl.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionMenu2;
    QAction *actionOpcion2_1;
    QAction *actionOpcion1_2;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_13;
    QLabel *label_myIp;
    QLabel *label_findedIp;
    QTabWidget *tabWidget;
    QWidget *tab;
    QGridLayout *gridLayout;
    FrameGL *glWidget;
    QGraphicsView *gvDepth;
    QGraphicsView *gvBarrido;
    QGraphicsView *gvVideo;
    QWidget *tab_2;
    QGroupBox *g_depth;
    QVBoxLayout *verticalLayout_3;
    QLabel *l_depth;
    QHBoxLayout *horizontalLayout_2;
    QSlider *slider_depth;
    QSpinBox *sb_depth;
    QCheckBox *cb_depth_send;
    QCheckBox *cb_depth_comp;
    QGroupBox *g_video;
    QVBoxLayout *verticalLayout_2;
    QLabel *l_video;
    QHBoxLayout *horizontalLayout;
    QSlider *slider_video;
    QSpinBox *sb_video;
    QCheckBox *cb_video_send;
    QCheckBox *cb_video_comp;
    QGroupBox *g_D;
    QGridLayout *gridLayout_4;
    QLabel *l_D_refresh;
    QHBoxLayout *horizontalLayout_3;
    QSlider *slider_D_refresh;
    QSpinBox *sb_D_refresh;
    QLabel *l_D_module;
    QHBoxLayout *horizontalLayout_4;
    QSlider *slider_D_module;
    QSpinBox *sb_D_module;
    QCheckBox *cb_D_3;
    QCheckBox *cb_D_2;
    QLabel *l_D_Ymin;
    QCheckBox *cb_D_barrido;
    QLabel *l_D_Ymax;
    QLabel *l_D_ecu;
    QLineEdit *le_D_ecu;
    QCheckBox *cb_D_comp;
    QSpinBox *sb_D_Ymin;
    QSpinBox *sb_D_Ymax;
    QGroupBox *g_led;
    QRadioButton *rb_off;
    QRadioButton *rb_green;
    QRadioButton *rb_red;
    QRadioButton *rb_yellow;
    QRadioButton *rb_blinkG;
    QRadioButton *rb_blinkRY;
    QGroupBox *g_limits;
    QGridLayout *gridLayout_3;
    QLabel *l_limits_Ymax;
    QLineEdit *le_limits_high;
    QLabel *l_limits_Ymin;
    QLineEdit *le_limits_kbaseangle;
    QLabel *l_limits_Zmax;
    QLabel *l_limits_high;
    QLabel *l_limits_baseang;
    QLabel *l_limits_kangle;
    QSpinBox *sb_limits_kangle;
    QSpinBox *sb_limits_Ymin;
    QSpinBox *sb_limits_Ymax;
    QSpinBox *sb_limits_Zmax;
    QGroupBox *groupBox;
    QCheckBox *cb_ir;
    QCheckBox *cb_depth;
    QCheckBox *cb_video;
    QCheckBox *cb_barrido;
    QCheckBox *cb_3;
    QCheckBox *cb_2;
    QHBoxLayout *horizontalLayout_14;
    QLabel *label_k;
    QComboBox *combo;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_12;
    QLabel *label_serverIp;
    QLineEdit *lineEdit;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_5;
    QLabel *labelS;
    QPushButton *pbGo;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_6;
    QLabel *labelDepth;
    QPushButton *pbDepth;
    QHBoxLayout *horizontalLayout_7;
    QLabel *labelVideo;
    QPushButton *pbVideo;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label3D;
    QPushButton *pb3D;
    QHBoxLayout *horizontalLayout_9;
    QLabel *label2D;
    QPushButton *pb2D;
    QHBoxLayout *horizontalLayout_10;
    QLabel *labelBarrido;
    QPushButton *pbBarrido;
    QHBoxLayout *horizontalLayout_11;
    QLabel *labelAccel;
    QPushButton *pbAccel;
    QSpacerItem *verticalSpacer_4;
    QTextEdit *textEdit;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(822, 610);
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
        horizontalLayout_13 = new QHBoxLayout();
        horizontalLayout_13->setSpacing(6);
        horizontalLayout_13->setObjectName(QStringLiteral("horizontalLayout_13"));
        label_myIp = new QLabel(centralWidget);
        label_myIp->setObjectName(QStringLiteral("label_myIp"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_myIp->sizePolicy().hasHeightForWidth());
        label_myIp->setSizePolicy(sizePolicy);
        label_myIp->setMinimumSize(QSize(40, 16));
        label_myIp->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_13->addWidget(label_myIp);

        label_findedIp = new QLabel(centralWidget);
        label_findedIp->setObjectName(QStringLiteral("label_findedIp"));
        label_findedIp->setMinimumSize(QSize(80, 0));
        label_findedIp->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_13->addWidget(label_findedIp);


        gridLayout_2->addLayout(horizontalLayout_13, 0, 0, 1, 1);

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
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        g_depth = new QGroupBox(tab_2);
        g_depth->setObjectName(QStringLiteral("g_depth"));
        g_depth->setGeometry(QRect(30, 20, 241, 121));
        verticalLayout_3 = new QVBoxLayout(g_depth);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        l_depth = new QLabel(g_depth);
        l_depth->setObjectName(QStringLiteral("l_depth"));

        verticalLayout_3->addWidget(l_depth);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        slider_depth = new QSlider(g_depth);
        slider_depth->setObjectName(QStringLiteral("slider_depth"));
        slider_depth->setMinimumSize(QSize(0, 22));
        slider_depth->setMinimum(33);
        slider_depth->setMaximum(2000);
        slider_depth->setPageStep(100);
        slider_depth->setOrientation(Qt::Horizontal);
        slider_depth->setInvertedAppearance(false);
        slider_depth->setInvertedControls(false);
        slider_depth->setTickPosition(QSlider::TicksBelow);
        slider_depth->setTickInterval(100);

        horizontalLayout_2->addWidget(slider_depth);

        sb_depth = new QSpinBox(g_depth);
        sb_depth->setObjectName(QStringLiteral("sb_depth"));
        sb_depth->setMinimum(33);
        sb_depth->setMaximum(2000);

        horizontalLayout_2->addWidget(sb_depth);


        verticalLayout_3->addLayout(horizontalLayout_2);

        cb_depth_send = new QCheckBox(g_depth);
        cb_depth_send->setObjectName(QStringLiteral("cb_depth_send"));

        verticalLayout_3->addWidget(cb_depth_send);

        cb_depth_comp = new QCheckBox(g_depth);
        cb_depth_comp->setObjectName(QStringLiteral("cb_depth_comp"));
        cb_depth_comp->setEnabled(false);

        verticalLayout_3->addWidget(cb_depth_comp);

        g_video = new QGroupBox(tab_2);
        g_video->setObjectName(QStringLiteral("g_video"));
        g_video->setGeometry(QRect(30, 150, 241, 121));
        verticalLayout_2 = new QVBoxLayout(g_video);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        l_video = new QLabel(g_video);
        l_video->setObjectName(QStringLiteral("l_video"));

        verticalLayout_2->addWidget(l_video);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        slider_video = new QSlider(g_video);
        slider_video->setObjectName(QStringLiteral("slider_video"));
        slider_video->setMinimumSize(QSize(0, 22));
        slider_video->setMinimum(33);
        slider_video->setMaximum(2000);
        slider_video->setPageStep(100);
        slider_video->setOrientation(Qt::Horizontal);
        slider_video->setTickPosition(QSlider::TicksBelow);
        slider_video->setTickInterval(100);

        horizontalLayout->addWidget(slider_video);

        sb_video = new QSpinBox(g_video);
        sb_video->setObjectName(QStringLiteral("sb_video"));
        sb_video->setMinimum(33);
        sb_video->setMaximum(2000);

        horizontalLayout->addWidget(sb_video);


        verticalLayout_2->addLayout(horizontalLayout);

        cb_video_send = new QCheckBox(g_video);
        cb_video_send->setObjectName(QStringLiteral("cb_video_send"));
        cb_video_send->setChecked(false);

        verticalLayout_2->addWidget(cb_video_send);

        cb_video_comp = new QCheckBox(g_video);
        cb_video_comp->setObjectName(QStringLiteral("cb_video_comp"));
        cb_video_comp->setEnabled(false);

        verticalLayout_2->addWidget(cb_video_comp);

        g_D = new QGroupBox(tab_2);
        g_D->setObjectName(QStringLiteral("g_D"));
        g_D->setGeometry(QRect(30, 280, 241, 231));
        gridLayout_4 = new QGridLayout(g_D);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        l_D_refresh = new QLabel(g_D);
        l_D_refresh->setObjectName(QStringLiteral("l_D_refresh"));

        gridLayout_4->addWidget(l_D_refresh, 0, 0, 1, 1);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        slider_D_refresh = new QSlider(g_D);
        slider_D_refresh->setObjectName(QStringLiteral("slider_D_refresh"));
        slider_D_refresh->setMinimumSize(QSize(0, 22));
        slider_D_refresh->setMinimum(33);
        slider_D_refresh->setMaximum(2000);
        slider_D_refresh->setOrientation(Qt::Horizontal);
        slider_D_refresh->setTickPosition(QSlider::TicksBelow);
        slider_D_refresh->setTickInterval(100);

        horizontalLayout_3->addWidget(slider_D_refresh);

        sb_D_refresh = new QSpinBox(g_D);
        sb_D_refresh->setObjectName(QStringLiteral("sb_D_refresh"));
        sb_D_refresh->setMinimum(33);
        sb_D_refresh->setMaximum(2000);

        horizontalLayout_3->addWidget(sb_D_refresh);


        gridLayout_4->addLayout(horizontalLayout_3, 1, 0, 1, 3);

        l_D_module = new QLabel(g_D);
        l_D_module->setObjectName(QStringLiteral("l_D_module"));

        gridLayout_4->addWidget(l_D_module, 2, 0, 1, 1);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        slider_D_module = new QSlider(g_D);
        slider_D_module->setObjectName(QStringLiteral("slider_D_module"));
        slider_D_module->setMinimumSize(QSize(0, 22));
        slider_D_module->setMinimum(1);
        slider_D_module->setMaximum(10);
        slider_D_module->setOrientation(Qt::Horizontal);
        slider_D_module->setTickPosition(QSlider::TicksBelow);
        slider_D_module->setTickInterval(1);

        horizontalLayout_4->addWidget(slider_D_module);

        sb_D_module = new QSpinBox(g_D);
        sb_D_module->setObjectName(QStringLiteral("sb_D_module"));
        sb_D_module->setMinimum(1);
        sb_D_module->setMaximum(10);
        sb_D_module->setValue(3);

        horizontalLayout_4->addWidget(sb_D_module);


        gridLayout_4->addLayout(horizontalLayout_4, 3, 0, 1, 3);

        cb_D_3 = new QCheckBox(g_D);
        cb_D_3->setObjectName(QStringLiteral("cb_D_3"));
        cb_D_3->setMinimumSize(QSize(70, 20));

        gridLayout_4->addWidget(cb_D_3, 4, 0, 1, 1);

        cb_D_2 = new QCheckBox(g_D);
        cb_D_2->setObjectName(QStringLiteral("cb_D_2"));
        cb_D_2->setMinimumSize(QSize(70, 20));

        gridLayout_4->addWidget(cb_D_2, 5, 0, 1, 1);

        l_D_Ymin = new QLabel(g_D);
        l_D_Ymin->setObjectName(QStringLiteral("l_D_Ymin"));
        l_D_Ymin->setMinimumSize(QSize(70, 20));
        l_D_Ymin->setMaximumSize(QSize(16777215, 20));
        l_D_Ymin->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(l_D_Ymin, 5, 1, 1, 1);

        cb_D_barrido = new QCheckBox(g_D);
        cb_D_barrido->setObjectName(QStringLiteral("cb_D_barrido"));
        cb_D_barrido->setMinimumSize(QSize(90, 20));
        cb_D_barrido->setMaximumSize(QSize(90, 20));
        cb_D_barrido->setChecked(true);

        gridLayout_4->addWidget(cb_D_barrido, 6, 0, 1, 1);

        l_D_Ymax = new QLabel(g_D);
        l_D_Ymax->setObjectName(QStringLiteral("l_D_Ymax"));
        l_D_Ymax->setMinimumSize(QSize(70, 20));
        l_D_Ymax->setMaximumSize(QSize(16777215, 20));
        l_D_Ymax->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(l_D_Ymax, 6, 1, 1, 1);

        l_D_ecu = new QLabel(g_D);
        l_D_ecu->setObjectName(QStringLiteral("l_D_ecu"));
        l_D_ecu->setMinimumSize(QSize(70, 20));
        l_D_ecu->setMaximumSize(QSize(16777215, 20));
        l_D_ecu->setAlignment(Qt::AlignCenter);

        gridLayout_4->addWidget(l_D_ecu, 7, 1, 1, 1);

        le_D_ecu = new QLineEdit(g_D);
        le_D_ecu->setObjectName(QStringLiteral("le_D_ecu"));
        le_D_ecu->setEnabled(false);
        le_D_ecu->setMinimumSize(QSize(50, 20));
        le_D_ecu->setMaximumSize(QSize(50, 20));
        le_D_ecu->setReadOnly(true);

        gridLayout_4->addWidget(le_D_ecu, 7, 2, 1, 1);

        cb_D_comp = new QCheckBox(g_D);
        cb_D_comp->setObjectName(QStringLiteral("cb_D_comp"));
        cb_D_comp->setEnabled(false);
        cb_D_comp->setMinimumSize(QSize(90, 20));
        cb_D_comp->setMaximumSize(QSize(90, 20));

        gridLayout_4->addWidget(cb_D_comp, 7, 0, 1, 1);

        sb_D_Ymin = new QSpinBox(g_D);
        sb_D_Ymin->setObjectName(QStringLiteral("sb_D_Ymin"));
        sb_D_Ymin->setMinimum(-1000);
        sb_D_Ymin->setMaximum(2000);

        gridLayout_4->addWidget(sb_D_Ymin, 5, 2, 1, 1);

        sb_D_Ymax = new QSpinBox(g_D);
        sb_D_Ymax->setObjectName(QStringLiteral("sb_D_Ymax"));
        sb_D_Ymax->setMinimum(-1000);
        sb_D_Ymax->setMaximum(2000);
        sb_D_Ymax->setValue(600);

        gridLayout_4->addWidget(sb_D_Ymax, 6, 2, 1, 1);

        l_D_refresh->raise();
        cb_D_3->raise();
        l_D_module->raise();
        cb_D_2->raise();
        cb_D_barrido->raise();
        l_D_Ymin->raise();
        l_D_ecu->raise();
        l_D_Ymax->raise();
        le_D_ecu->raise();
        cb_D_comp->raise();
        sb_D_Ymin->raise();
        sb_D_Ymax->raise();
        g_led = new QGroupBox(tab_2);
        g_led->setObjectName(QStringLiteral("g_led"));
        g_led->setGeometry(QRect(500, 20, 111, 171));
        rb_off = new QRadioButton(g_led);
        rb_off->setObjectName(QStringLiteral("rb_off"));
        rb_off->setGeometry(QRect(9, 26, 32, 16));
        rb_off->setChecked(true);
        rb_green = new QRadioButton(g_led);
        rb_green->setObjectName(QStringLiteral("rb_green"));
        rb_green->setGeometry(QRect(9, 50, 46, 16));
        rb_red = new QRadioButton(g_led);
        rb_red->setObjectName(QStringLiteral("rb_red"));
        rb_red->setGeometry(QRect(9, 74, 34, 16));
        rb_yellow = new QRadioButton(g_led);
        rb_yellow->setObjectName(QStringLiteral("rb_yellow"));
        rb_yellow->setGeometry(QRect(9, 98, 48, 16));
        rb_blinkG = new QRadioButton(g_led);
        rb_blinkG->setObjectName(QStringLiteral("rb_blinkG"));
        rb_blinkG->setGeometry(QRect(9, 122, 70, 16));
        rb_blinkRY = new QRadioButton(g_led);
        rb_blinkRY->setObjectName(QStringLiteral("rb_blinkRY"));
        rb_blinkRY->setGeometry(QRect(9, 146, 91, 16));
        g_limits = new QGroupBox(tab_2);
        g_limits->setObjectName(QStringLiteral("g_limits"));
        g_limits->setGeometry(QRect(360, 280, 174, 174));
        gridLayout_3 = new QGridLayout(g_limits);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        l_limits_Ymax = new QLabel(g_limits);
        l_limits_Ymax->setObjectName(QStringLiteral("l_limits_Ymax"));
        l_limits_Ymax->setMinimumSize(QSize(100, 20));
        l_limits_Ymax->setMaximumSize(QSize(100, 20));
        l_limits_Ymax->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(l_limits_Ymax, 5, 0, 1, 1);

        le_limits_high = new QLineEdit(g_limits);
        le_limits_high->setObjectName(QStringLiteral("le_limits_high"));
        le_limits_high->setMinimumSize(QSize(50, 20));
        le_limits_high->setMaximumSize(QSize(50, 20));

        gridLayout_3->addWidget(le_limits_high, 0, 1, 1, 1);

        l_limits_Ymin = new QLabel(g_limits);
        l_limits_Ymin->setObjectName(QStringLiteral("l_limits_Ymin"));
        l_limits_Ymin->setMinimumSize(QSize(100, 20));
        l_limits_Ymin->setMaximumSize(QSize(100, 20));
        l_limits_Ymin->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(l_limits_Ymin, 4, 0, 1, 1);

        le_limits_kbaseangle = new QLineEdit(g_limits);
        le_limits_kbaseangle->setObjectName(QStringLiteral("le_limits_kbaseangle"));
        le_limits_kbaseangle->setEnabled(false);
        le_limits_kbaseangle->setMinimumSize(QSize(50, 20));
        le_limits_kbaseangle->setMaximumSize(QSize(50, 20));
        le_limits_kbaseangle->setReadOnly(false);

        gridLayout_3->addWidget(le_limits_kbaseangle, 1, 1, 2, 1);

        l_limits_Zmax = new QLabel(g_limits);
        l_limits_Zmax->setObjectName(QStringLiteral("l_limits_Zmax"));
        l_limits_Zmax->setMinimumSize(QSize(100, 20));
        l_limits_Zmax->setMaximumSize(QSize(100, 20));
        l_limits_Zmax->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(l_limits_Zmax, 6, 0, 1, 1);

        l_limits_high = new QLabel(g_limits);
        l_limits_high->setObjectName(QStringLiteral("l_limits_high"));
        l_limits_high->setMinimumSize(QSize(100, 20));
        l_limits_high->setMaximumSize(QSize(100, 20));

        gridLayout_3->addWidget(l_limits_high, 0, 0, 1, 1);

        l_limits_baseang = new QLabel(g_limits);
        l_limits_baseang->setObjectName(QStringLiteral("l_limits_baseang"));
        l_limits_baseang->setMinimumSize(QSize(100, 20));
        l_limits_baseang->setMaximumSize(QSize(100, 20));

        gridLayout_3->addWidget(l_limits_baseang, 1, 0, 1, 1);

        l_limits_kangle = new QLabel(g_limits);
        l_limits_kangle->setObjectName(QStringLiteral("l_limits_kangle"));
        l_limits_kangle->setMinimumSize(QSize(100, 20));
        l_limits_kangle->setMaximumSize(QSize(100, 20));

        gridLayout_3->addWidget(l_limits_kangle, 2, 0, 2, 1);

        sb_limits_kangle = new QSpinBox(g_limits);
        sb_limits_kangle->setObjectName(QStringLiteral("sb_limits_kangle"));
        sb_limits_kangle->setMinimum(-25);
        sb_limits_kangle->setMaximum(25);

        gridLayout_3->addWidget(sb_limits_kangle, 3, 1, 1, 1);

        sb_limits_Ymin = new QSpinBox(g_limits);
        sb_limits_Ymin->setObjectName(QStringLiteral("sb_limits_Ymin"));
        sb_limits_Ymin->setMinimum(-1000);
        sb_limits_Ymin->setMaximum(2000);
        sb_limits_Ymin->setValue(-500);

        gridLayout_3->addWidget(sb_limits_Ymin, 4, 1, 1, 1);

        sb_limits_Ymax = new QSpinBox(g_limits);
        sb_limits_Ymax->setObjectName(QStringLiteral("sb_limits_Ymax"));
        sb_limits_Ymax->setMinimum(-1000);
        sb_limits_Ymax->setMaximum(2000);
        sb_limits_Ymax->setValue(1500);

        gridLayout_3->addWidget(sb_limits_Ymax, 5, 1, 1, 1);

        sb_limits_Zmax = new QSpinBox(g_limits);
        sb_limits_Zmax->setObjectName(QStringLiteral("sb_limits_Zmax"));
        sb_limits_Zmax->setMinimum(500);
        sb_limits_Zmax->setMaximum(9000);
        sb_limits_Zmax->setValue(8000);

        gridLayout_3->addWidget(sb_limits_Zmax, 6, 1, 1, 1);

        groupBox = new QGroupBox(tab_2);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(360, 20, 111, 171));
        cb_ir = new QCheckBox(groupBox);
        cb_ir->setObjectName(QStringLiteral("cb_ir"));
        cb_ir->setEnabled(false);
        cb_ir->setGeometry(QRect(10, 140, 101, 31));
        cb_depth = new QCheckBox(groupBox);
        cb_depth->setObjectName(QStringLiteral("cb_depth"));
        cb_depth->setGeometry(QRect(10, 20, 101, 16));
        cb_depth->setChecked(false);
        cb_video = new QCheckBox(groupBox);
        cb_video->setObjectName(QStringLiteral("cb_video"));
        cb_video->setGeometry(QRect(10, 40, 101, 16));
        cb_video->setChecked(false);
        cb_barrido = new QCheckBox(groupBox);
        cb_barrido->setObjectName(QStringLiteral("cb_barrido"));
        cb_barrido->setGeometry(QRect(10, 100, 101, 16));
        cb_barrido->setChecked(true);
        cb_3 = new QCheckBox(groupBox);
        cb_3->setObjectName(QStringLiteral("cb_3"));
        cb_3->setGeometry(QRect(10, 60, 101, 16));
        cb_2 = new QCheckBox(groupBox);
        cb_2->setObjectName(QStringLiteral("cb_2"));
        cb_2->setGeometry(QRect(10, 80, 101, 16));
        tabWidget->addTab(tab_2, QString());

        gridLayout_2->addWidget(tabWidget, 0, 1, 15, 1);

        horizontalLayout_14 = new QHBoxLayout();
        horizontalLayout_14->setSpacing(6);
        horizontalLayout_14->setObjectName(QStringLiteral("horizontalLayout_14"));
        label_k = new QLabel(centralWidget);
        label_k->setObjectName(QStringLiteral("label_k"));
        sizePolicy.setHeightForWidth(label_k->sizePolicy().hasHeightForWidth());
        label_k->setSizePolicy(sizePolicy);
        label_k->setMinimumSize(QSize(30, 0));
        label_k->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_14->addWidget(label_k);

        combo = new QComboBox(centralWidget);
        combo->setObjectName(QStringLiteral("combo"));
        combo->setEnabled(false);
        combo->setMinimumSize(QSize(80, 20));
        combo->setMaximumSize(QSize(130, 16777215));

        horizontalLayout_14->addWidget(combo);


        gridLayout_2->addLayout(horizontalLayout_14, 1, 0, 1, 1);

        verticalSpacer_2 = new QSpacerItem(80, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_2, 2, 0, 1, 1);

        horizontalLayout_12 = new QHBoxLayout();
        horizontalLayout_12->setSpacing(6);
        horizontalLayout_12->setObjectName(QStringLiteral("horizontalLayout_12"));
        label_serverIp = new QLabel(centralWidget);
        label_serverIp->setObjectName(QStringLiteral("label_serverIp"));
        QSizePolicy sizePolicy2(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(label_serverIp->sizePolicy().hasHeightForWidth());
        label_serverIp->setSizePolicy(sizePolicy2);
        label_serverIp->setMinimumSize(QSize(40, 20));
        label_serverIp->setMaximumSize(QSize(50, 25));

        horizontalLayout_12->addWidget(label_serverIp);

        lineEdit = new QLineEdit(centralWidget);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));
        lineEdit->setMinimumSize(QSize(90, 20));
        lineEdit->setMaximumSize(QSize(90, 20));
        lineEdit->setReadOnly(true);

        horizontalLayout_12->addWidget(lineEdit);


        gridLayout_2->addLayout(horizontalLayout_12, 3, 0, 1, 1);

        verticalSpacer_3 = new QSpacerItem(88, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_3, 4, 0, 1, 1);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        labelS = new QLabel(centralWidget);
        labelS->setObjectName(QStringLiteral("labelS"));
        labelS->setEnabled(true);
        QSizePolicy sizePolicy3(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(labelS->sizePolicy().hasHeightForWidth());
        labelS->setSizePolicy(sizePolicy3);
        labelS->setMinimumSize(QSize(0, 0));
        labelS->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(labelS);

        pbGo = new QPushButton(centralWidget);
        pbGo->setObjectName(QStringLiteral("pbGo"));
        pbGo->setEnabled(false);
        QSizePolicy sizePolicy4(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy4.setHorizontalStretch(0);
        sizePolicy4.setVerticalStretch(0);
        sizePolicy4.setHeightForWidth(pbGo->sizePolicy().hasHeightForWidth());
        pbGo->setSizePolicy(sizePolicy4);
        pbGo->setMinimumSize(QSize(70, 20));
        pbGo->setMaximumSize(QSize(16777215, 16777215));

        horizontalLayout_5->addWidget(pbGo);


        gridLayout_2->addLayout(horizontalLayout_5, 5, 0, 1, 1);

        verticalSpacer_5 = new QSpacerItem(80, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_5, 6, 0, 1, 1);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        labelDepth = new QLabel(centralWidget);
        labelDepth->setObjectName(QStringLiteral("labelDepth"));

        horizontalLayout_6->addWidget(labelDepth);

        pbDepth = new QPushButton(centralWidget);
        pbDepth->setObjectName(QStringLiteral("pbDepth"));
        pbDepth->setMinimumSize(QSize(70, 20));

        horizontalLayout_6->addWidget(pbDepth);


        gridLayout_2->addLayout(horizontalLayout_6, 7, 0, 1, 1);

        horizontalLayout_7 = new QHBoxLayout();
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        labelVideo = new QLabel(centralWidget);
        labelVideo->setObjectName(QStringLiteral("labelVideo"));

        horizontalLayout_7->addWidget(labelVideo);

        pbVideo = new QPushButton(centralWidget);
        pbVideo->setObjectName(QStringLiteral("pbVideo"));
        pbVideo->setMinimumSize(QSize(70, 20));

        horizontalLayout_7->addWidget(pbVideo);


        gridLayout_2->addLayout(horizontalLayout_7, 8, 0, 1, 1);

        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label3D = new QLabel(centralWidget);
        label3D->setObjectName(QStringLiteral("label3D"));

        horizontalLayout_8->addWidget(label3D);

        pb3D = new QPushButton(centralWidget);
        pb3D->setObjectName(QStringLiteral("pb3D"));
        pb3D->setMinimumSize(QSize(70, 20));

        horizontalLayout_8->addWidget(pb3D);


        gridLayout_2->addLayout(horizontalLayout_8, 9, 0, 1, 1);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setSpacing(6);
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        label2D = new QLabel(centralWidget);
        label2D->setObjectName(QStringLiteral("label2D"));

        horizontalLayout_9->addWidget(label2D);

        pb2D = new QPushButton(centralWidget);
        pb2D->setObjectName(QStringLiteral("pb2D"));
        pb2D->setMinimumSize(QSize(70, 20));

        horizontalLayout_9->addWidget(pb2D);


        gridLayout_2->addLayout(horizontalLayout_9, 10, 0, 1, 1);

        horizontalLayout_10 = new QHBoxLayout();
        horizontalLayout_10->setSpacing(6);
        horizontalLayout_10->setObjectName(QStringLiteral("horizontalLayout_10"));
        labelBarrido = new QLabel(centralWidget);
        labelBarrido->setObjectName(QStringLiteral("labelBarrido"));

        horizontalLayout_10->addWidget(labelBarrido);

        pbBarrido = new QPushButton(centralWidget);
        pbBarrido->setObjectName(QStringLiteral("pbBarrido"));
        pbBarrido->setMinimumSize(QSize(70, 20));

        horizontalLayout_10->addWidget(pbBarrido);


        gridLayout_2->addLayout(horizontalLayout_10, 11, 0, 1, 1);

        horizontalLayout_11 = new QHBoxLayout();
        horizontalLayout_11->setSpacing(6);
        horizontalLayout_11->setObjectName(QStringLiteral("horizontalLayout_11"));
        labelAccel = new QLabel(centralWidget);
        labelAccel->setObjectName(QStringLiteral("labelAccel"));

        horizontalLayout_11->addWidget(labelAccel);

        pbAccel = new QPushButton(centralWidget);
        pbAccel->setObjectName(QStringLiteral("pbAccel"));
        pbAccel->setMinimumSize(QSize(70, 20));

        horizontalLayout_11->addWidget(pbAccel);


        gridLayout_2->addLayout(horizontalLayout_11, 12, 0, 1, 1);

        verticalSpacer_4 = new QSpacerItem(80, 10, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer_4, 13, 0, 1, 1);

        textEdit = new QTextEdit(centralWidget);
        textEdit->setObjectName(QStringLiteral("textEdit"));
        textEdit->setMaximumSize(QSize(130, 16777215));

        gridLayout_2->addWidget(textEdit, 14, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 822, 15));
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
        label_myIp->setText(QApplication::translate("MainWindow", "my ip:", 0));
        label_findedIp->setText(QApplication::translate("MainWindow", "localhost", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "views", 0));
        g_depth->setTitle(QApplication::translate("MainWindow", "depth", 0));
        l_depth->setText(QApplication::translate("MainWindow", "refresh   ms/frame", 0));
        cb_depth_send->setText(QApplication::translate("MainWindow", "send", 0));
        cb_depth_comp->setText(QApplication::translate("MainWindow", "compressed", 0));
        g_video->setTitle(QApplication::translate("MainWindow", "video", 0));
        l_video->setText(QApplication::translate("MainWindow", "refresh   ms/frame", 0));
        cb_video_send->setText(QApplication::translate("MainWindow", "send", 0));
        cb_video_comp->setText(QApplication::translate("MainWindow", "compressed", 0));
        g_D->setTitle(QApplication::translate("MainWindow", "3D points", 0));
        l_D_refresh->setText(QApplication::translate("MainWindow", "refresh ms/frame", 0));
        l_D_module->setText(QApplication::translate("MainWindow", "module", 0));
        cb_D_3->setText(QApplication::translate("MainWindow", "send 3D", 0));
        cb_D_2->setText(QApplication::translate("MainWindow", "send 2D", 0));
        l_D_Ymin->setText(QApplication::translate("MainWindow", "Ymin (mm)", 0));
        cb_D_barrido->setText(QApplication::translate("MainWindow", "send barrido", 0));
        l_D_Ymax->setText(QApplication::translate("MainWindow", "Ymax (mm)", 0));
        l_D_ecu->setText(QApplication::translate("MainWindow", "ecu", 0));
        le_D_ecu->setText(QApplication::translate("MainWindow", " = Zmax", 0));
        cb_D_comp->setText(QApplication::translate("MainWindow", "compressed", 0));
        g_led->setTitle(QApplication::translate("MainWindow", "led", 0));
        rb_off->setText(QApplication::translate("MainWindow", "off", 0));
        rb_green->setText(QApplication::translate("MainWindow", "green", 0));
        rb_red->setText(QApplication::translate("MainWindow", "red", 0));
        rb_yellow->setText(QApplication::translate("MainWindow", "yellow", 0));
        rb_blinkG->setText(QApplication::translate("MainWindow", "blink green", 0));
        rb_blinkRY->setText(QApplication::translate("MainWindow", "blink red/yellow", 0));
        g_limits->setTitle(QApplication::translate("MainWindow", "limits", 0));
        l_limits_Ymax->setText(QApplication::translate("MainWindow", "Ymax (mm)", 0));
        le_limits_high->setText(QApplication::translate("MainWindow", "0.56", 0));
        l_limits_Ymin->setText(QApplication::translate("MainWindow", "Ymin (mm)", 0));
        le_limits_kbaseangle->setText(QString());
        l_limits_Zmax->setText(QApplication::translate("MainWindow", "Zmax (mm)", 0));
        l_limits_high->setText(QApplication::translate("MainWindow", "kinect camera high", 0));
        l_limits_baseang->setText(QApplication::translate("MainWindow", "kinect base angle", 0));
        l_limits_kangle->setText(QApplication::translate("MainWindow", "kinect angle (deg)", 0));
        groupBox->setTitle(QApplication::translate("MainWindow", "show", 0));
        cb_ir->setText(QApplication::translate("MainWindow", "IR", 0));
        cb_depth->setText(QApplication::translate("MainWindow", "depth shown", 0));
        cb_video->setText(QApplication::translate("MainWindow", "video shown", 0));
        cb_barrido->setText(QApplication::translate("MainWindow", "barrrido shown", 0));
        cb_3->setText(QApplication::translate("MainWindow", "3d shown", 0));
        cb_2->setText(QApplication::translate("MainWindow", "2d shown", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "data", 0));
        label_k->setText(QApplication::translate("MainWindow", "select k", 0));
        combo->setCurrentText(QString());
        label_serverIp->setText(QApplication::translate("MainWindow", "server ip:", 0));
        labelS->setText(QApplication::translate("MainWindow", "10003", 0));
        pbGo->setText(QApplication::translate("MainWindow", "Go", 0));
        labelDepth->setText(QApplication::translate("MainWindow", "10004", 0));
        pbDepth->setText(QApplication::translate("MainWindow", "depth", 0));
        labelVideo->setText(QApplication::translate("MainWindow", "10005", 0));
        pbVideo->setText(QApplication::translate("MainWindow", "video", 0));
        label3D->setText(QApplication::translate("MainWindow", "10006", 0));
        pb3D->setText(QApplication::translate("MainWindow", "3D", 0));
        label2D->setText(QApplication::translate("MainWindow", "10007", 0));
        pb2D->setText(QApplication::translate("MainWindow", "2D", 0));
        labelBarrido->setText(QApplication::translate("MainWindow", "10008", 0));
        pbBarrido->setText(QApplication::translate("MainWindow", "barrido", 0));
        labelAccel->setText(QApplication::translate("MainWindow", "10009", 0));
        pbAccel->setText(QApplication::translate("MainWindow", "accel", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
