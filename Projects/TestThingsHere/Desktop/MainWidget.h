#ifndef MainWidgetH
#define MainWidgetH

#include <QWidget>

#include <AudioDeviceDriver.h>
#include <AudioDeviceInputCV.h>
#include <AudioDeviceOutputOscilator.h>

class MainWidget : public QWidget
{
   Q_OBJECT
public:
   MainWidget(QWidget* parent);

private:
   void audioLoop(const float& callbackRate);

private:
   AudioDevice::Driver driver;
   AudioDevice::InputCV pitch;
   AudioDevice::OutputOscilator oscilator;
};

#endif // NOT MainWidgetH
