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
#            cvwork.cpp \
#            settings.cpp

HEADERS     += mainwindow.h \
            maincalib.h \
            apikinect/apikinect2.h \
#            cvwork.h \
#            settings.h

FORMS       += mainwindow.ui

INCLUDEPATH += /usr/include
INCLUDEPATH += $$PWD/apikinect/include


LIBS += -L$$PWD/apikinect/lib/  -lfreenect \
                                -lusb-1.0 \
                                -lopencv_core \
#                                -lopencv_calib3d \
                                -lopencv_highgui \
#                                -lopencv_imgproc #\
#                                -lopencv_contrib \
#                                -lopencv_features2d\
#                                -lopencv_flann \
#                                -lopencv_gpu \
#                                -lopencv_legacy \
#                                -lopencv_ml \
#                                -lopencv_objdetect \
#                                -lopencv_ocl \
#                                -lopencv_photo \
#                                -lopencv_stitching \
#                                -lopencv_superres \
#                                -lopencv_ts \
#                                -lopencv_video
#                                -lopencv_videostab \
#                                -lopencv_core


DEPENDPATH  += $$PWD/apikinect/include

PRE_TARGETDEPS += $$PWD/apikinect/lib/libusb-1.0.a
PRE_TARGETDEPS += $$PWD/apikinect/lib/libfreenect.a

#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_calib3d.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_contrib.a
PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_highgui.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_features2d.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_flann.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_gpu.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_imgproc.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_legacy.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_ml.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_objdetect.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_ocl.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_photo.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_stitching.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_superres.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_ts.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_video.a
#PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_videostab.a
PRE_TARGETDEPS += $$PWD/apikinect/lib/libopencv_core.a


QMAKE_CXXFLAGS  += -std=gnu++11
