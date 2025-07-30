#include "FilterBank.h"

#include "Harmonizer.h"

FilterBank::FilterBank()
   : filters{}
{
}

void FilterBank::init(const float& sampleRate)
{
   for (uint8_t index = 0; index < 4; index++)
   {
      filters[index].init(sampleRate, FilterMode::BandPass);
      filters[index].setDrive(0.1);
   }
}

void FilterBank::update(const float& resonance, const Notes& notes)
{
   for (uint8_t index = 0; index < 4; index++)
   {
      filters[index].setResonance(resonance);
      filters[index].setFrequency(notes[index].frequency);
   }
}

float FilterBank::changeSound(const float& in)
{
   float out = 0.0;
   for (uint8_t index = 0; index < 4; index++)
   {
      out += 0.25 * filters[index].changeSound(in);
   }
   return out;
}
