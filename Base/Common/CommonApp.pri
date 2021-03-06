# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h

HEADERS += \
   $$PWD/AppSettings.h \
   $$PWD/ConsoleApplication.h \
   $$PWD/FileSettings.h \
   $$PWD/RootStorage.h

SOURCES += \
   $$PWD/AppSettings.cpp \
   $$PWD/ConsoleApplication.cpp \
   $$PWD/FileSettings.cpp \
   $$PWD/RootStorage.cpp

CONFIG(midi):include($$PWD/MidiDevice/MidiDevice.pri)
CONFIG(audio):include($$PWD/AudioDevice/AudioDevice.pri)

CONFIG += c++17

INCLUDEPATH += $$PWD


