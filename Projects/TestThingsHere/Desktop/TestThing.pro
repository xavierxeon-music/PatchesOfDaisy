TARGET = TestThing
TEMPLATE = app

QT += widgets svg
CONFIG += audio
DESTDIR = ../bin


include(../../../Base/DesktopApp.pri)

HEADERS += \
   MainWidget.h \

SOURCES += \
   MainWidget.cpp \
