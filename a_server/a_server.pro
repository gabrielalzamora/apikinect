#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T10:36:55
#
#-------------------------------------------------

QT       += core gui network opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = a_server
TEMPLATE = app


SOURCES += main.cpp\
           mainwindow.cpp \
           apikinect.cpp \
           attendclient.cpp \
           framegl.cpp \
           data.cpp

HEADERS  += mainwindow.h \
            apikinect.h \
            attendclient.h \
            framegl.h \
            data.h \
    typekinect.h

FORMS    += mainwindow.ui

# -L path to libraries -l libraries needed to compile 
# delete initial lib_ from filename, i.e. libfreenect.so ->> -lfreenect

LIBS    += -L /usr/lib/x86_64-linux-gnu -lfreenect -lusb-1.0 -lGLU -lGLEW

# INCLUDEPATH += tell QT where do you hide your include's
INCLUDEPATH += /usr/include/libusb-1.0

# due to problems compiling std::vector std::mutex and so on?????
QMAKE_CXXFLAGS  += -std=gnu++11
