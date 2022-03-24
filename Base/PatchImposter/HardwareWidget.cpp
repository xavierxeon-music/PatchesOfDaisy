#include "HardwareWidget.h"

#include <QKeyEvent>
#include <QTimer>

#include "ImposterDaisyPatch.h"

HardwareWidget::HardwareWidget(QWidget* parent, Imposter::DaisyPatch* imposterPatch)
   : QWidget(parent)
   , imposterPatch(imposterPatch)

   , displayPixmap(130, 66)
   , audioInPixmap()
   , audioOutPixmap()
{
   for (uint8_t channel = 0; channel < 4; channel++)
   {
      audioInPixmap[channel] = QPixmap(320, 50);
      audioOutPixmap[channel] = QPixmap(320, 50);
   }

   setupUi(this);
   daisyInfo->setFrameShape(QFrame::NoFrame);
   //connect(daisyInfo, &DaisyLabel::signalDobuleClicked, this, &HardwareWidget::slotOpenPythonDialog);

   imposterPatch->encoder.setup(encoderForwardButton, encoderBackButton, encoderClickButton, encoderPressButton);

   imposterPatch->controls[0].setup(cvInKnob1, cvInLabel1);
   imposterPatch->controls[1].setup(cvInKnob2, cvInLabel2);
   imposterPatch->controls[2].setup(cvInKnob3, cvInLabel3);
   imposterPatch->controls[3].setup(cvInKnob4, cvInLabel4);

   imposterPatch->gate_input[0].setup(gateInCheck1);
   imposterPatch->gate_input[1].setup(gateInCheck2);

   imposterPatch->display.setup(this, &displayPixmap);

   imposterPatch->audioHandle.inChannel[0].setup(oscilatorLabel1, &audioInPixmap[0]);
   imposterPatch->audioHandle.inChannel[1].setup(oscilatorLabel2, &audioInPixmap[1]);
   imposterPatch->audioHandle.inChannel[2].setup(oscilatorLabel3, &audioInPixmap[2]);
   imposterPatch->audioHandle.inChannel[3].setup(oscilatorLabel4, &audioInPixmap[3]);

   imposterPatch->audioHandle.outChannel[0].setup(&audioOutPixmap[0]);
   imposterPatch->audioHandle.outChannel[1].setup(&audioOutPixmap[1]);
   imposterPatch->audioHandle.outChannel[2].setup(&audioOutPixmap[2]);
   imposterPatch->audioHandle.outChannel[3].setup(&audioOutPixmap[3]);

   imposterPatch->seed.setup(ledCheck);

   updateState();

   const int audioIntervall = 10;

   QTimer* audioTimer = new QTimer(this);
   connect(audioTimer, &QTimer::timeout, this, &HardwareWidget::slotNextAudioStep);
   audioTimer->start(audioIntervall);

   QTimer* otherTimer = new QTimer(this);
   connect(otherTimer, &QTimer::timeout, this, &HardwareWidget::slotNextNonAudioStep);
   otherTimer->start(audioIntervall * 5);

   setFixedSize(740, 600);
}

void HardwareWidget::refreshDisplay()
{
   auto refresh = [](const QPixmap& pixmap, QLabel* label)
   {
      const int width = label->width();
      const int height = label->height();

      const QPixmap content = pixmap.scaled(width, height, Qt::KeepAspectRatio, Qt::SmoothTransformation);
      label->setPixmap(content);
   };

   refresh(displayPixmap, displayLabel);

   refresh(audioInPixmap[0], waveInLabel1);
   refresh(audioInPixmap[1], waveInLabel2);
   refresh(audioInPixmap[2], waveInLabel3);
   refresh(audioInPixmap[3], waveInLabel4);

   refresh(audioOutPixmap[0], waveOutLabel1);
   refresh(audioOutPixmap[1], waveOutLabel2);
   refresh(audioOutPixmap[2], waveOutLabel3);
   refresh(audioOutPixmap[3], waveOutLabel4);
}

void HardwareWidget::updateState()
{
   const float sampleRate = imposterPatch->AudioSampleRate();
   sampleRateLabel->setText(QString::number(sampleRate));

   const size_t blockSize = imposterPatch->AudioBlockSize();
   blockSizeLabel->setText(QString::number(blockSize));
}

void HardwareWidget::applyOutput(const Output& output)
{
   cvOutSlider1->setValue(output.controlVoltages[0] * 1000);
   cvOutLabel1->setText(QString::number(output.controlVoltages[0], 'f', 2) + "V");

   cvOutSlider2->setValue(output.controlVoltages[1] * 1000);
   cvOutLabel2->setText(QString::number(output.controlVoltages[1], 'f', 2) + "V");

   gateOutCheck->setChecked(output.gate);
   ledCheck->setChecked(output.led);
}

void HardwareWidget::slotNextAudioStep()
{
   imposterPatch->executeAudioStep();
}

void HardwareWidget::slotNextNonAudioStep()
{
   static bool everyOther = true;

   imposterPatch->executeNonAudioStep(everyOther);
   everyOther = !everyOther;
}

void HardwareWidget::keyReleaseEvent(QKeyEvent* ke)
{
   QWidget::keyReleaseEvent(ke);

   if (ke->key() == Qt::Key_A)
      encoderBackButton->click();
   else if (ke->key() == Qt::Key_D)
      encoderForwardButton->click();
   else if (ke->key() == Qt::Key_K)
      encoderClickButton->click();
   else if (ke->key() == Qt::Key_P)
      encoderPressButton->click();
}
