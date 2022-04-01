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

linux {
   LIBS += -ljack
}

QT += network

INCLUDEPATH += $$PWD/Include

HEADERS += \
   $$PWD/Include/MidiBridge.h \
   $$PWD/Include/MidiBridge.hpp \
   $$PWD/Include/MidiDevice.h \
   $$PWD/Include/MidiTunnelClient.h \
   $$PWD/Include/MidiTunnelServer.h \
   $$PWD/Private/MidiTunnelSocket.h \
   $$PWD/RtMidi4/RtMidi4.h

SOURCES += \
   $$PWD/MidiBridge.cpp \
   $$PWD/MidiDevice.cpp \
   $$PWD/MidiTunnelClient.cpp \
   $$PWD/MidiTunnelServer.cpp \
   $$PWD/MidiTunnelSocket.cpp \
   $$PWD/RtMidi4/RtMidi4.cpp
