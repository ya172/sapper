QT       += core gui
QT += widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = sapper_2
TEMPLATE = app


DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += c++11

SOURCES += \
        main.cpp \
        pushbutton.cpp \
        sapper.cpp


HEADERS += \
        pushbutton.h \
        sapper.h


RESOURCES += \
    images.qrc


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
#-------------------------------------------------
#
# Project created by QtCreator 2012-03-01T15:26:24
#
#-------------------------------------------------

