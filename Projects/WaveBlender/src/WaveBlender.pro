TARGET = WaveBlender
TEMPLATE = app

include(../../../Base/PatchImposterApp.pri)

HEADERS += \
    CvMapping.h \
    OverlayCv.h \
    PageChannel.h \
    RandomWalkTables.h \
    CustomTable.h \
    WaveBlender.h

SOURCES += \
    CvMapping.cpp \
    OverlayCv.cpp \
    PageChannel.cpp \
    RandomWalkTables.cpp \
    CustomTable.cpp \
    WaveBlender.cpp

