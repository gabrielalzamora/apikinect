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
    apikinect/apikinect.cpp \
    apikinect/attendclient.cpp \
    apikinect/configdata.cpp \
    apikinect/framegl.cpp \
    apikinect/maincore.cpp

HEADERS  += mainwindow.h \
    typekinect.h \
    apikinect/include/GL/gl.h \
    apikinect/include/GL/glu.h \
    apikinect/include/GL/glu_mangle.h \
    apikinect/include/libfreenect.h \
    apikinect/include/libfreenect.hpp \
    apikinect/include/libusb.h \
    apikinect/apikinect.h \
    apikinect/attendclient.h \
    apikinect/configdata.h \
    apikinect/framegl.h \
    apikinect/maincore.h \
    apikinect/typekinect.h

FORMS    += mainwindow.ui

# -L path to libraries -l libraries needed to compile 
# delete initial lib_ from filename, i.e. libfreenect.so ->> -lfreenect

LIBS    += -L ./apikinect/lib -lfreenect -lusb-1.0 -lGLU -lGLEW

# INCLUDEPATH += tell QT where do you hide your include's
INCLUDEPATH += ./apikinect/include

# due to problems compiling std::vector std::mutex and so on
QMAKE_CXXFLAGS  += -std=gnu++11
