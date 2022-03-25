# mark as imposter
DEFINES += NON_DAISY_DEVICE

macx:include($$PWD/mac.pri)

include($$PWD/PatchImposter/PatchImposter.pri)
include($$PWD/PatchDevice/PatchDevice.pri)
include($$PWD/Common/JSONTools/JSONTools.pri)
include($$PWD/Common/Remember/Remember.pri)
include($$PWD/Common/MusicTools/MusicTools.pri)

# midi
macos{
   DEFINES += __MACOSX_CORE__
   LIBS += \
     -framework CoreMIDI \
     -framework CoreAudio \
     -framework CoreFoundation
}

win32{
   DEFINES += __WINDOWS_MM__
   LIBS += Winmm.lib
}

RT_MIDI_PATH=$$PWD/Common/MidiBridge/RtMidi4
INCLUDEPATH += $$RT_MIDI_PATH

HEADERS += \
   $$RT_MIDI_PATH/RtMidi4.h

SOURCES += \
   $$RT_MIDI_PATH/RtMidi4.cpp
