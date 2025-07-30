#include "RhythmBank.h"

RhythmBank::RhythmBank()
   : Remember::Container()
   , rhythms(this)
{
}

void RhythmBank::init()
{
   for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
      rhythms[midiChannel].init();
}

Rhythm& RhythmBank::getRhythm(const uint8_t& midiChannel)
{
   return rhythms[midiChannel];
}

void RhythmBank::clockTick()
{
   for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
      rhythms[midiChannel].clockTick();
}

void RhythmBank::clockReset()
{
   for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
      rhythms[midiChannel].clockReset();
}
