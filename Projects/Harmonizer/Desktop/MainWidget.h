#ifndef MainWidgetH
#define MainWidgetH

#include "ui_MainWidget.h"
#include <QWidget>

#include <QMutex>

#include <AudioDevice.h>
#include <Effect/Revoice.h>
#include <Sound/AudioFile.h>

class MainWidget : public QWidget, private Ui::MainWidget
{
   Q_OBJECT
public:
   MainWidget(QWidget* parent);

private slots:
   void slotLoad();
   void slotSave();

private:
   void audioCallback(const void* inputBuffer, void* outputBuffer, const AudioDevice::Frame& framesPerBuffer);

private:
   AudioDevice::Driver driver;
   QMutex mutex;

   Revoice revoiceLeft;
   Revoice revoiceRight;

   AudioFile::Meta meta;
   Data playData;
   size_t playhead;
};

#endif // NOT MainWidgetH
