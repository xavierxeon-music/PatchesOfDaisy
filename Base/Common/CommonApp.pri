# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h

HEADERS += \
   $$PWD/AppSettings.h \
   $$PWD/ConsoleApplication.h \
   $$PWD/FileStorage.h

SOURCES += \
   $$PWD/AppSettings.cpp \
   $$PWD/ConsoleApplication.cpp \
   $$PWD/FileStorage.cpp

CONFIG(midi):include($$PWD/MidiDevice/MidiDevice.pri)
CONFIG(audio):include($$PWD/AudioDevice/AudioDevice.pri)

INCLUDEPATH += $$PWD


