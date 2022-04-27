# mark as imposter
DEFINES += NON_DAISY_DEVICE

macx:include($$PWD/mac.pri)

include($$PWD/PatchImposter/PatchImposter.pri)
include($$PWD/PatchDevice/PatchDevice.pri)
include($$PWD/Common/JSONTools/JSONTools.pri)
include($$PWD/Common/Remember/Remember.pri)
include($$PWD/Common/MusicTools/MusicTools.pri)
include($$PWD/Common/MidiDevice/MidiDevice.pri)

# pre compiled headers
CONFIG += precompile_header
PRECOMPILED_HEADER += $$PWD/Common/Global.h

