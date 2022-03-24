#include "ImposterEncoder.h"

Imposter::Encoder::Encoder()
   : QObject(nullptr)
   , incValue(0)
   , clicked(false)
   , pressed(false)
{
}

void Imposter::Encoder::Debounce()
{
   // do nothing
}

int32_t Imposter::Encoder::Increment() const
{
   const int32_t value = incValue;
   incValue = 0;

   return value;
}

bool Imposter::Encoder::FallingEdge() const
{
   if (!clicked)
      return false;

   clicked = false;
   return true;
}

bool Imposter::Encoder::Pressed() const
{
   return pressed;
}

float Imposter::Encoder::TimeHeldMs() const
{
   if (pressed)
   {
      pressed = false;
      return 1000.0;
   }
   return 100.0;
}

void Imposter::Encoder::setup(QPushButton* plusButton, QPushButton* minusButton, QPushButton* clickButton, QPushButton* pressButton)
{
   connect(plusButton, &QAbstractButton::clicked, this, &Encoder::plus);
   connect(minusButton, &QAbstractButton::clicked, this, &Encoder::minus);
   connect(clickButton, &QAbstractButton::clicked, this, &Encoder::click);
   connect(pressButton, &QAbstractButton::clicked, this, &Encoder::press);
}

void Imposter::Encoder::plus()
{
   incValue = 1;
}

void Imposter::Encoder::minus()
{
   incValue = -1;
}

void Imposter::Encoder::click()
{
   clicked = true;
}

void Imposter::Encoder::press()
{
   clicked = true;
   pressed = true;
}
