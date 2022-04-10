QMAKE_APPLE_DEVICE_ARCHS = $$QMAKE_HOST.arch # do not build universal binaries
QMAKE_MACOSX_DEPLOYMENT_TARGET = 12.0

CONFIG(audio):QMAKE_POST_LINK = $$PWD/mac_audio_plist.py $$TARGET $$OUT_PWD $$DESTDIR

