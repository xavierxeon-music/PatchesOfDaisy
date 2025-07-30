TARGET = Schaltzentrale
TEMPLATE = app

include(../../../Base/PatchImposterApp.pri)

HEADERS += \
    Main.h \
    PageAbstract.h \
    PageGate.h \
    PageOverview.h \
    PageRamp.h

SOURCES += \
    Main.cpp \
    PageAbstract.cpp \
    PageGate.cpp \
    PageOverview.cpp \
    PageRamp.cpp

