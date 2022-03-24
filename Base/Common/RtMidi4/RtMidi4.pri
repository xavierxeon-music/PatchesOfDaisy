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

INCLUDEPATH += $$PWD

HEADERS += \
   $$PWD/RtMidi4.h

SOURCES += \
   $$PWD/RtMidi4.cpp
   
