TARGET = AudioDeviceDummyLib
TEMPLATE = lib

# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/../../Global.h

include(../AudioDevice.pri)
include(../../MusicTools/MusicTools.pri)

macx:QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_HOST.arch

