TARGET = BeatMaster
TEMPLATE = app

include(../../Base/PatchImposterApp.pri)

HEADERS += \
    Gate.h \
    Icons.h \
    Main.h \
    OverlayCopy.h \
    PageAbstract.h \
    PageGates.h \
    PageOverview.h \
    PageRhythm.h \
    Rhythm.h \
    RhythmBank.h 

SOURCES += \
    Gate.cpp \
    Icons.cpp \
    IconsData.cpp \
    Main.cpp \
    OverlayCopy.cpp \
    PageAbstract.cpp \
    PageGates.cpp \
    PageOverview.cpp \
    PageRhythm.cpp \
    Rhythm.cpp \
    RhythmBank.cpp 
