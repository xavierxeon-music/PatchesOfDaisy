# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h

HEADERS += \
   $$PWD/FileStorage.h \
   $$PWD/SettingsUI.h

SOURCES += \
   $$PWD/FileStorage.cpp \
   $$PWD/SettingsUI.cpp

CONFIG(midi):include($$PWD/MidiDevice/MidiDevice.pri)
CONFIG(audio):include($$PWD/AudioDevice/AudioDevice.pri)

INCLUDEPATH += $$PWD


