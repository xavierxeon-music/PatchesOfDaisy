unix {
    system($$PWD/portaudio/build.sh) # will exit if already built
    LIBS += -L$$PWD/portaudio/install/lib -lportaudio
}

INCLUDEPATH += $$PWD/Include

HEADERS += \
   $$PWD/Include/AudioDevice.h \
   $$PWD/Include/AudioDeviceDriver.h \
   $$PWD/Include/AudioDeviceInputCV.h \
   $$PWD/Include/AudioDeviceInputGate.h \
   $$PWD/Include/AudioDeviceInputRaw.h \
   $$PWD/Include/AudioDeviceInputsTempo.h \
   $$PWD/Include/AudioDeviceOutputCV.h \
   $$PWD/Include/AudioDeviceOutputGate.h \
   $$PWD/Include/AudioDeviceOutputOscilator.h \
   $$PWD/Include/AudioDeviceOutputRaw.h \
   $$PWD/Private/AudioDeviceBuffers.h \
   $$PWD/Private/AudioDeviceCommon.h \
   $$PWD/Private/AudioDeviceDriver.hpp

SOURCES += \
   $$PWD/AudioDeviceBuffers.cpp \
   $$PWD/AudioDeviceDriver.cpp \
   $$PWD/AudioDeviceInputCV.cpp \
   $$PWD/AudioDeviceInputGate.cpp \
   $$PWD/AudioDeviceInputRaw.cpp \
   $$PWD/AudioDeviceInputsTempo.cpp \
   $$PWD/AudioDeviceOutputCV.cpp \
   $$PWD/AudioDeviceOutputGate.cpp \
   $$PWD/AudioDeviceOutputOscilator.cpp \
   $$PWD/AudioDeviceOutputRaw.cpp
