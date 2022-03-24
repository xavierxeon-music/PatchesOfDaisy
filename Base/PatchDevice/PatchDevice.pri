INCLUDEPATH += \
   $$PWD \
   $$PWD/Include

HEADERS += \
    $$PWD/Include/Abstract/Abstract.h \
    $$PWD/Include/Abstract/AbstractPage.h \
    $$PWD/Include/Abstract/AbstractPageBase.h \
    $$PWD/Include/Abstract/AbstractPageManager.h \
    $$PWD/Include/Abstract/AbstractPageOverlay.h \
    $$PWD/Include/Abstract/AbstractPatch.h \
    $$PWD/Include/Abstract/AbstractSettings.h \
    $$PWD/Include/Global.h \
    $$PWD/Include/Graphics/BitMask.h \
    $$PWD/Include/Graphics/IndicatorBlock.h \
    $$PWD/Include/Graphics/IndicatorPie.h \
    $$PWD/Include/Graphics/IndicatorSlider.h \
    $$PWD/Include/Midi/MidiHandlerBase.h \
    $$PWD/Include/Midi/MidiHandlerInternal.h \
    $$PWD/Include/Midi/MidiHandlerUSB.h \
    $$PWD/Include/Midi/MidiInterfaceFlameCC.h \
    $$PWD/Include/Music/ClockedTempo.h \
    $$PWD/Include/Storage/FlashSettings.h \
    $$PWD/Include/Storage/LogFile.h \
    $$PWD/Include/Storage/SDCard.h \
    $$PWD/Include/Storage/SDCardSettings.h \
    $$PWD/Include/Tools/Timer.h \
    
HEADERS += \    
   $$PWD/BitMask.hpp \
   $$PWD/ClockedTempo.hpp \
   $$PWD/FlashSettings.hpp \
   $$PWD/IndicatorBlock.hpp \
   $$PWD/IndicatorPie.hpp \
   $$PWD/IndicatorSlider.hpp \
   $$PWD/MidiHandlerBase.hpp \
   $$PWD/MidiHandlerInternal.hpp \
   $$PWD/MidiHandlerUSB.hpp \
   $$PWD/MidiInterfaceFlameCC.hpp \
   $$PWD/SDCard.hpp \
   $$PWD/SDCardSettings.hpp

SOURCES += \
   $$PWD/AbstractPage.cpp \
   $$PWD/AbstractPageBase.cpp \
   $$PWD/AbstractPageManager.cpp \
   $$PWD/AbstractPageOverlay.cpp \
   $$PWD/AbstractPatch.cpp \
   $$PWD/AbstractSettings.cpp \
   $$PWD/LogFile.cpp \
   $$PWD/SDCard.cpp \
   $$PWD/Timer.cpp
