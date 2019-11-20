#-------------------------------------------------
#
# Project created by QtCreator 2019-11-09T18:43:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
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

CONFIG += c++17

SOURCES += \
        ImageProcess.cpp \
        main.cpp \
        widget.cpp

HEADERS += \
        ImageProcess.h \
        Timer.h \
        widget.h

FORMS += \
        widget.ui \
        widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target



    win32: LIBS += -L$$PWD/Dependency/lib/ -lopencv_world411

    INCLUDEPATH += $$PWD/Dependency/
    DEPENDPATH += $$PWD/Dependency/

    win32:!win32-g++: PRE_TARGETDEPS += $$PWD/Dependency/lib/opencv_world411.lib
    else:win32-g++: PRE_TARGETDEPS += $$PWD/Dependency/lib/libopencv_world411.a
