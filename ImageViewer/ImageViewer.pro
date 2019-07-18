#-------------------------------------------------
#
# Project created by QtCreator 2019-03-01T13:46:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageViewer
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

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    myframeitem.cpp \
    imageprocessor.cpp \
    dialogtransform.cpp \
    dialogcontraststretch.cpp \
    dialoglinear.cpp \
    dialoggaussfilter.cpp \
    dialogbilateralfilter.cpp \
    dialogmedianfilter.cpp \
    dialogmorphology.cpp \
    fft.cpp \
    dialogaffinetransform.cpp \
    dialogfrequencydomainfilter.cpp

HEADERS += \
        mainwindow.h \
    myframeitem.h \
    imageprocessor.h \
    dialogtransform.h \
    dialogcontraststretch.h \
    dialoglinear.h \
    dialoggaussfilter.h \
    dialogbilateralfilter.h \
    dialogmedianfilter.h \
    dialogmorphology.h \
    fft.h \
    matrix.h \
    dialogaffinetransform.h \
    dialogfrequencydomainfilter.h

FORMS += \
        mainwindow.ui \
    dialogtransform.ui \
    dialogcontraststretch.ui \
    dialoglinear.ui \
    dialoggaussfilter.ui \
    dialogbilateralfilter.ui \
    dialogmedianfilter.ui \
    dialogmorphology.ui \
    dialogaffinetransform.ui \
    dialogfrequencydomainfilter.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
