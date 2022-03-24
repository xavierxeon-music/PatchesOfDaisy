TARGET = MidiBridgeDummyLib
TEMPLATE = lib

# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/../../Global.h

include(../MidiBridge.pri)
include(../../MusicTools/MusicTools.pri)
include(../../Remember/Remember.pri)

macx:QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_HOST.arch

