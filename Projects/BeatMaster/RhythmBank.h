#ifndef RhythmBankH
#define RhythmBankH

#include <Remember.h>

#include "Rhythm.h"

class RhythmBank : public Remember::Container
{
public:
   RhythmBank();

public:
   void init();
   Rhythm& getRhythm(const uint8_t& midiChannel);

   // gates
   void clockTick();
   void clockReset();

private: // things to remeber
   using RhythmArray_ = Remember::RefArray<Rhythm, 16>;

private:
   RhythmArray_ rhythms;
};

#endif // RhythmBankH
