# ----------------------------------------------------
# This file is generated by the Qt Visual Studio Add-in.
# ------------------------------------------------------

# This is a reminder that you are using a generated .pro file.
# Remove it when you are finished editing this file.

# message("You are running qmake on a generated .pro file. This may not work!")

TEMPLATE = app

QT += core gui

TARGET = camCapture

LIBS += -L./thirdparty/lib \
        -lopencv_core249d \
        -lopencv_highgui249d \
        -lopencv_imgproc249d \
        -lopencv_ml249d

INCLUDEPATH += ./thirdparty/include \
               ./thirdparty/include/opencv \
               ./thirdparty/include/opencv2


HEADERS += camcapture.h
SOURCES += camcapture.cpp \
           main.cpp

FORMS += camcapture.ui