#-------------------------------------------------
#
# Project created by QtCreator 2019-02-15T00:47:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GaugeReader
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

# RC_ICONS = "CallButton.ico"

SOURCES += \
        main.cpp \
        src/mainwindow.cpp \
    src/gaugereader.cpp

HEADERS += \
        src/mainwindow.h \
    src/gaugereader.h

FORMS += \
        ui/mainwindow.ui

win32:CONFIG(release, debug|release): LIBS += -LD:/opencv_sdk/opencv3/build/x64/vc15/lib/ -lopencv_world345
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv_sdk/opencv3/build/x64/vc15/lib/ -lopencv_world345d

win32:CONFIG(release, debug|release): LIBS += -LD:/opencv_sdk/opencv3/build/x64/vc15/lib/ -lopencv_xfeatures2d345
else:win32:CONFIG(debug, debug|release): LIBS += -LD:/opencv_sdk/opencv3/build/x64/vc15/lib/ -lopencv_xfeatures2d345d

INCLUDEPATH += D:/opencv_sdk/opencv3/build/include
DEPENDPATH += D:/opencv_sdk/opencv3/build/include

