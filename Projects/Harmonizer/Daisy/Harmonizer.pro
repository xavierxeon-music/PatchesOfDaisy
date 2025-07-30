TARGET = Harmonizer
TEMPLATE = app

include(../../../Base/PatchImposterApp.pri)
#AddPythonFile(Harmonizer.py)

HEADERS += \
    FilterBank.h \
    Common.h \
    PageOverview.h \
    Harmonizer.h

SOURCES += \
    FilterBank.cpp \
    PageOverview.cpp \
    Harmonizer.cpp

