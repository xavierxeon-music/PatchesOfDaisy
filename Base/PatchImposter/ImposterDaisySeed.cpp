#include "ImposterDaisySeed.h"

Imposter::DaisySeed::DaisySeed()
   : QObject(nullptr)
   , ledOn(false)
   , radioButton(nullptr)
{

}

void Imposter::DaisySeed::SetLed(bool on)
{
   ledOn = on;

   if (radioButton)
      radioButton->setChecked(ledOn);
}

void Imposter::DaisySeed::setup(QRadioButton* radioButton)
{
   this->radioButton = radioButton;
   radioButton->setChecked(ledOn);
}
