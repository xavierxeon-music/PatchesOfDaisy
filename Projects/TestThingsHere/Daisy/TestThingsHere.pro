TARGET = TestThingsHere
TEMPLATE = app

include(../../Base/PatchImposterApp.pri)

HEADERS += \
   CvSwitch.h \
   TestThingsHere.h \
   ThePage.h

SOURCES += \
   CvSwitch.cpp \
   TestThingsHere.cpp \
   ThePage.cpp
