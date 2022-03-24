# mark as imposter
DEFINES += NON_DAISY_DEVICE

macx:include($$PWD/mac.pri)

include($$PWD/PatchImposter/PatchImposter.pri)
include($$PWD/PatchDevice/PatchDevice.pri)
include($$PWD/Common/JSONTools/JSONTools.pri)
include($$PWD/Common/RtMidi4/RtMidi4.pri)
include($$PWD/Common/Remember/Remember.pri)
include($$PWD/Common/MusicTools/MusicTools.pri)
