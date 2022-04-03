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
   DEFINES += __UNIX_JACK__
   LIBS += -ljack
}

QT += network

INCLUDEPATH += $$PWD/Include

HEADERS += \
   $$PWD/Include/Midi/MidiDeviceInput.h \
   $$PWD/Include/Midi/MidiDeviceOutput.h \
   $$PWD/Include/Midi/MidiToolBridge.h \
   $$PWD/Include/Midi/MidiToolGate.h \
   $$PWD/Include/Midi/MidiTunnelClient.h \
   $$PWD/Include/Midi/MidiTunnelServer.h \
   $$PWD/Private/MidiTunnelSocket.h \
   $$PWD/Private/MidiToolBridge.hpp \
   $$PWD/RtMidi4/RtMidi4.h

SOURCES += \
   $$PWD/MidiDeviceInput.cpp \
   $$PWD/MidiDeviceOutput.cpp \
   $$PWD/MidiToolBridge.cpp \
   $$PWD/MidiToolGate.cpp \
   $$PWD/MidiTunnelClient.cpp \
   $$PWD/MidiTunnelServer.cpp \
   $$PWD/MidiTunnelSocket.cpp \
   $$PWD/RtMidi4/RtMidi4.cpp
