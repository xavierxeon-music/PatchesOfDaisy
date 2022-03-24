INCLUDEPATH += $$PWD

unix {
    system($$PWD/portaudio/build.sh) # will exit if already built
    INCLUDEPATH += $$PWD/portaudio/install/include
    LIBS += -L$$PWD/portaudio/install/lib -lportaudio
}

HEADERS += \
    $$PWD/AudioDeviceInputsTempo.h \
    $$PWD/Private/AudioDeviceBuffers.h \
    $$PWD/Private/AudioDeviceCommon.h \
    $$PWD/Private/AudioDeviceDriver.hpp \
    $$PWD/AudioDevice.h \
    $$PWD/AudioDeviceDriver.h \
    $$PWD/AudioDeviceInputCV.h \
    $$PWD/AudioDeviceInputGate.h \
    $$PWD/AudioDeviceInputRaw.h \
    $$PWD/AudioDeviceOutputCV.h \
    $$PWD/AudioDeviceOutputGate.h \
    $$PWD/AudioDeviceOutputOscilator.h \
    $$PWD/AudioDeviceOutputRaw.h

SOURCES += \
   $$PWD/AudioDeviceInputsTempo.cpp \
    $$PWD/Private/AudioDeviceBuffers.cpp \
    $$PWD/AudioDeviceDriver.cpp \
    $$PWD/AudioDeviceInputCV.cpp \
    $$PWD/AudioDeviceInputGate.cpp \
    $$PWD/AudioDeviceInputRaw.cpp \
    $$PWD/AudioDeviceOutputCV.cpp \
    $$PWD/AudioDeviceOutputGate.cpp \
    $$PWD/AudioDeviceOutputOscilator.cpp \
    $$PWD/AudioDeviceOutputRaw.cpp \
