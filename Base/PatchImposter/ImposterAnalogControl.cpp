#include "ImposterAnalogControl.h"

#include <Tools/Range.h>

#include "Settings.h"

Imposter::AnalogControl::AnalogControl()
   : QObject(nullptr)
   , name()
   , dial(nullptr)
   , valueLabel(nullptr)
   , currentKnob(0.0)
   , currentRemote(0.0)
{
}

void Imposter::AnalogControl::init(uint8_t channel)
{
   name = "knob" + QString::number(channel);
}

float Imposter::AnalogControl::Value() const
{
   float current = Range::clamp<float>(currentKnob + currentRemote, 0.0, 1.0);

   if (valueLabel)
   {
      QString valueText = QString::number(current, 'f', 2);
      valueLabel->setText(valueText);
   }
   return current;
}

void Imposter::AnalogControl::setup(QDial* dial, QLabel* valueLabel)
{
   this->valueLabel = valueLabel;
   this->dial = dial;

   Settings settings("controls");
   currentKnob = settings.real(name);

   dial->setRange(0, 100);
   dial->setValue(0);
   dial->setSingleStep(1);
   dial->setValue(100 * Value());
   connect(dial, &QDial::valueChanged, this, &AnalogControl::dialChanged);
}

void Imposter::AnalogControl::setRemote(const float& value)
{
   currentRemote = value;
}

void Imposter::AnalogControl::dialChanged(double value)
{
   currentKnob = value / 100.0;

   Settings settings("controls");
   settings.write(name, currentKnob);
}
