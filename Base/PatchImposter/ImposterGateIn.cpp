#include "ImposterGateIn.h"

Imposter::GateIn::GateIn()
   : QObject(nullptr)
   , active(false)
   , radioButton(nullptr)
{

}

bool Imposter::GateIn::Trig()
{
   if (radioButton)
      radioButton->setChecked(active);

   return active;
}

void Imposter::GateIn::setup(QRadioButton* radioButton)
{
   this->radioButton = radioButton;
}

void Imposter::GateIn::activateRemote(bool on)
{
   active = on;
}
