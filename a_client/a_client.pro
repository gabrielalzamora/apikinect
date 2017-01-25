#-------------------------------------------------
#
#-------------------------------------------------

QT       += core gui network opengl widgets

TARGET = a_client
TEMPLATE = app

SOURCES += main.cpp\
           mainwindow.cpp\
    apikinect/mainclient.cpp \
    apikinect/configdata.cpp \
    apikinect/framegl.cpp \

HEADERS  += mainwindow.h\
    apikinect/mainclient.h \
    apikinect/configdata.h \
    apikinect/typekinect.h \
    apikinect/framegl.h

FORMS    += mainwindow.ui

unix:!macx: LIBS += -L$$PWD/apikinect/lib/ -lGLU

INCLUDEPATH += $$PWD/apikinect/include
DEPENDPATH += $$PWD/apikinect/include

unix:!macx: PRE_TARGETDEPS += $$PWD/apikinect/lib/libGLU.a

QMAKE_CXXFLAGS  += -std=gnu++11
