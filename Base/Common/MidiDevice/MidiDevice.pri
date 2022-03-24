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

INCLUDEPATH=$$PWD

HEADERS += \
   $$PWD/MidiDevice.h \
   $$PWD/MidiDevice.hpp \
   $$PWD/RtMidi4/RtMidi4.h

SOURCES += \
   $$PWD/MidiDevice.cpp \
   $$PWD/RtMidi4/RtMidi4.cpp