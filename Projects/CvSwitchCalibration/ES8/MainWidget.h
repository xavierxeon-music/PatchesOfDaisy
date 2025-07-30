#ifndef MainWidgetH
#define MainWidgetH

#include "ui_MainWidget.h"
#include <QWidget>

#include <AudioDevice.h>
#include <Blocks/CvSwitch.h>

class MainWidget : public QWidget, private Ui::MainWidget
{
   Q_OBJECT
public:
   MainWidget();

private slots:
   void slotDevieChanged();
   void slotPrevStep();
   void slotNexStep();
   void slotManulOffsetChanged();

private:
   void udpateUiAndSend();

private:
   CvSwitch::StandardDevices::TargetDevice device;
   CvSwitch cvSwitch;
   uint8_t index;

   float voltage;
   float offset;
   bool manualOffset;

   AudioDevice::Driver audioDriver;
   AudioDevice::InputCV offsetInput;
   AudioDevice::OutputCV switchOutput;
};

#endif // MainWidgetH
