INCLUDEPATH += $$PWD

# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Global.h

HEADERS += \
   $$PWD/SettingsUI.h

SOURCES += \
   $$PWD/SettingsUI.cpp

CONFIG(midi):include($$PWD/MidiDevice/MidiDevice.pri)
CONFIG(audio):include($$PWD/AudioDevice/AudioDevice.pri)


