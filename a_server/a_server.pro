#-------------------------------------------------
#
# Project created by QtCreator 2016-07-08T10:36:55
#
# y no sabes cuantas veces he modificado...
#-------------------------------------------------

QT       += core gui network opengl widgets

TARGET = a_server
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp \
    apikinect/mainserver.cpp \
    apikinect/apikinect.cpp \
    apikinect/configdata.cpp \
    apikinect/framegl.cpp \
    apikinect/attendclient.cpp

HEADERS  += mainwindow.h \
    apikinect/mainserver.h \
    apikinect/apikinect.h \
    apikinect/configdata.h \
    apikinect/framegl.h \
    apikinect/typekinect.h \
    apikinect/attendclient.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/apikinect/lib/ -lusb-1.0 -lfreenect -lGLU

INCLUDEPATH += $$PWD/apikinect/include
DEPENDPATH += $$PWD/apikinect/include

unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libusb-1.0.a
unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libfreenect.a
unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libGLU.a

QMAKE_CXXFLAGS  += -std=gnu++11
