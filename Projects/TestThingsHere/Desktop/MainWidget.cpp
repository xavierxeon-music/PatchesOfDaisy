#include "MainWidget.h"

#include <QApplication>

MainWidget::MainWidget(QWidget* parent)
   : QWidget(parent)
   , driver(AudioDevice::Common::Device::ES8, AudioDevice::Common::SampleRate::Normal)
   , pitch(&driver, 3)
   , oscilator(&driver, 3, Standard::Waveform::Sine)
{
   driver.registerAudioLoopFunction(this, &MainWidget::audioLoop);
}

void MainWidget::audioLoop(const float& callbackRate)
{
   Q_UNUSED(callbackRate)

   const float voltage = pitch.getVoltage();
   const float frequency = TableOscilator::frequencyFromCV(voltage);
   oscilator.setFrequency(frequency);
}

// main function

int main(int argc, char** argv)
{
   QApplication app(argc, argv);

   MainWidget mw(nullptr);
   mw.show();

   return app.exec();
}
