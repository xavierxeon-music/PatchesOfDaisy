TARGET = MidiAccesories
TEMPLATE = app

include(../../Base/PatchImposterApp.pri)

HEADERS += \
   MidiAccesories.h \
   PageAbstract.h \
   PageFlameCC.h

SOURCES += \
   MidiAccesories.cpp \
   PageAbstract.cpp \
   PageFlameCC.cpp
