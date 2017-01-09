#-------------------------------------------------
#
#
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a_calib
TEMPLATE = app


SOURCES     += main.cpp\
            mainwindow.cpp \
            maincalib.cpp \
            apikinect/apikinect2.cpp \
    cvwork.cpp

HEADERS     += mainwindow.h \
            maincalib.h \
            apikinect/apikinect2.h \
    cvwork.h

FORMS       += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/apikinect/lib/  -lfreenect -lusb-1.0

INCLUDEPATH += $$PWD/apikinect/include
DEPENDPATH  += $$PWD/apikinect/include

unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libusb-1.0.a
unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libfreenect.a

QMAKE_CXXFLAGS  += -std=gnu++11

