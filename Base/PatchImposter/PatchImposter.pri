QT += widgets network svg
CONFIG += c++11
#macos:CONFIG -= app_bundle

# app icon
win32:RC_ICONS = $$PWD/Resources/Imposter.ico
macos:ICON = $$PWD/Resources/Imposter.icns

# python files
CONFIG += file_copies
COPIES += PythonScripts
win32:PythonScripts.path = $$OUT_PWD
unix:PythonScripts.path = $$OUT_PWD
macos:PythonScripts.path = $$OUT_PWD/$${TARGET}.app/Contents/MacOS

defineTest(AddPythonFile) {
   isEmpty($$1): return

   PythonScripts.files += $$1
   export(PythonScripts.files)
}

# source code
INCLUDEPATH += \
    $$PWD

HEADERS += \
    $$PWD/AudioBufferLabel.h \
    $$PWD/AudioChannelAbstract.h \
    $$PWD/AudioChannelInput.h \
    $$PWD/AudioChannelOutput.h \
    $$PWD/DaisyLabel.h \
    $$PWD/HardwareWidget.h \
    $$PWD/ImposterAnalogControl.h \
    $$PWD/ImposterAudioHandle.h \
    $$PWD/ImposterDaisyPatch.h \
    $$PWD/ImposterDaisySeed.h \
    $$PWD/ImposterEncoder.h \
    $$PWD/ImposterFontDef.h \
    $$PWD/ImposterGateIn.h \
    $$PWD/ImposterMidiEvent.h \
    $$PWD/ImposterMidiUsbHandler.h \
    $$PWD/ImposterOledDisplay.h \
    $$PWD/LogWidget.h \
    $$PWD/MainWindow.h \
    $$PWD/PythonProcess.h \
    $$PWD/PythonServer.h \
    $$PWD/PythonWidget.h \
    $$PWD/Settings.h \
    $$PWD/TempoWidget.h


SOURCES += \
    $$PWD/AudioBufferLabel.cpp \
    $$PWD/AudioChannelAbstract.cpp \
    $$PWD/AudioChannelInput.cpp \
    $$PWD/AudioChannelOutput.cpp \
    $$PWD/DaisyLabel.cpp \
    $$PWD/HardwareWidget.cpp \
    $$PWD/ImposterAnalogControl.cpp \
    $$PWD/ImposterAudioHandle.cpp \
    $$PWD/ImposterDaisyPatch.cpp \
    $$PWD/ImposterDaisySeed.cpp \
    $$PWD/ImposterEncoder.cpp \
    $$PWD/ImposterFontDef.cpp \
    $$PWD/ImposterGateIn.cpp \
    $$PWD/ImposterMidiUsbHandler.cpp \
    $$PWD/ImposterOledDisplay.cpp \
    $$PWD/LogWidget.cpp \
    $$PWD/MainWindow.cpp \
    $$PWD/PythonProcess.cpp \
    $$PWD/PythonServer.cpp \
    $$PWD/PythonWidget.cpp \
    $$PWD/Settings.cpp \
    $$PWD/TempoWidget.cpp

FORMS += \
   $$PWD/HardwareWidget.ui \
   $$PWD/LogWidget.ui \
   $$PWD/PythonWidget.ui \
   $$PWD/TempoWidget.ui

RESOURCES += \
   $$PWD/Resources/Resources.qrc
