# mark as imposter
DEFINES += NON_DAISY_DEVICE

macx:include($$PWD/mac.pri)

include($$PWD/SeedCompanion/SeedCompanion.pri)
include($$PWD/Common/CommonUI.pri)
include($$PWD/Common/JSONTools/JSONTools.pri)
include($$PWD/Common/Remember/Remember.pri)
include($$PWD/Common/MusicTools/MusicTools.pri)

