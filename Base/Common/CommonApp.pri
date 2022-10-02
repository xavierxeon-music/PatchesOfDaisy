# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h

HEADERS += \
   $$PWD/AppSettings.h \
   $$PWD/ColorPicker.h \
   $$PWD/ConsoleApplication.h \
   $$PWD/FileSettings.h \
   $$PWD/HubClient.h \
   $$PWD/RootStorage.h

SOURCES += \
   $$PWD/AppSettings.cpp \
   $$PWD/ColorPicker.cpp \
   $$PWD/ConsoleApplication.cpp \
   $$PWD/FileSettings.cpp \
   $$PWD/HubClient.cpp \
   $$PWD/RootStorage.cpp

CONFIG(midi):include($$PWD/MidiDevice/MidiDevice.pri)
CONFIG(audio):include($$PWD/AudioDevice/AudioDevice.pri)

CONFIG += c++17

INCLUDEPATH += $$PWD


