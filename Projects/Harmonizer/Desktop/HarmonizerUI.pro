TARGET = HarmonizerUI
TEMPLATE = app

QT += widgets
CONFIG += audio
DESTDIR = ../bin

#win32:RC_ICONS = ApplicaionIcon/TimeLord.ico
#macos:ICON = ApplicaionIcon/TimeLord.icns

include(../../../Base/DesktopApp.pri)

HEADERS += \
    MainWidget.h

SOURCES += \
    MainWidget.cpp

FORMS += \
    MainWidget.ui

