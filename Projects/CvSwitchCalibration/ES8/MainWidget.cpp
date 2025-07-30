#include "MainWidget.h"

#include <QApplication>
#include <QTimer>

#include <Tools/Variable.h>

using Device = CvSwitch::StandardDevices::TargetDevice;

MainWidget::MainWidget()
   : QWidget(nullptr)
   , device(CvSwitch::StandardDevices::TargetDevice::TotalRecall)
   , cvSwitch(CvSwitch::StandardDevices::channelCount(device), CvSwitch::StandardDevices::offsetMap(device))
   , index(7)
   , voltage(0.0)
   , offset(0.0)
   , manualOffset(false)
   , audioDriver("ES-8")
   , offsetInput(&audioDriver, 0)
   , switchOutput(&audioDriver, 0)
{
   setupUi(this);
   setWindowTitle("CvSwitch Calibration");

   const std::vector<Device> deviceList = {Device::MimeticDigitalis4, Device::MimeticDigitalis16, Device::ViceVirga2, Device::ViceVirga4, Device::ViceVirga8, Device::TotalRecall};
   int startIndex = 0;
   for (const Device& combBoxDevice : qAsConst(deviceList))
   {
      const QString deviceName = QString::fromStdString(CvSwitch::StandardDevices::name(combBoxDevice));
      const int deviceIndex = deviceCombo->count();
      deviceCombo->addItem(deviceName, QVariant::fromValue(combBoxDevice));
      if (combBoxDevice == device)
         startIndex = deviceIndex;
   }

   deviceCombo->setCurrentIndex(startIndex);
   manualOffsetCheck->setChecked(manualOffset);

   connect(deviceCombo, &QComboBox::currentIndexChanged, this, &MainWidget::slotDevieChanged);
   connect(prevStepButton, &QPushButton::clicked, this, &MainWidget::slotPrevStep);
   connect(nexStepButton, &QPushButton::clicked, this, &MainWidget::slotNexStep);
   connect(manualOffsetCheck, &QPushButton::clicked, this, &MainWidget::slotManulOffsetChanged);

   udpateUiAndSend();

   QTimer* updateTimer = new QTimer(this);
   connect(updateTimer, &QTimer::timeout, this, &MainWidget::udpateUiAndSend);
   updateTimer->start(100);
}

void MainWidget::slotDevieChanged()
{
   Device device = deviceCombo->currentData().value<Device>();
   cvSwitch = CvSwitch::StandardDevices::create(device);
}

void MainWidget::slotPrevStep()
{
   Variable::Integer<uint8_t> var(index, 0, cvSwitch.getMaxIndex() - 1, true);
   var.change(false);
}

void MainWidget::slotNexStep()
{
   Variable::Integer<uint8_t> var(index, 0, cvSwitch.getMaxIndex() - 1, true);
   var.change(true);
}

void MainWidget::slotManulOffsetChanged()
{
   manualOffset = manualOffsetCheck->isChecked();
}

void MainWidget::udpateUiAndSend()
{
   stepInfo->display(index);
   offset = offsetInput.getVoltage();
   cvOffsetSlider->setValue(10 * offset);

   float appliedVoltage = 0.0;

   if (manualOffset)
   {
      voltage = cvSwitch.map(index, false);
      appliedVoltage = offset + voltage;

      offsetInfo->show();
      offsetInfo->display(offset);
   }
   else
   {
      voltage = cvSwitch.map(index, true);
      appliedVoltage = voltage;

      offsetInfo->hide();
   }

   cvInfo->display(appliedVoltage);
   switchOutput.setVoltage(appliedVoltage);
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWidget mw;
   mw.show();

   return app.exec();
}
