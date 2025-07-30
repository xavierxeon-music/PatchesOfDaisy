#ifndef RelativeEightH
#define RelativeEightH

// base
#include <Abstract/AbstractPatch.h>
#include <Storage/FlashSettings.h>

#include <Music/ClockedTempo.h>
#include <Remember.h>
#include <Sound/GateOscilator.h>

#include "Channel.h"
#include "Sequence.h"

class RelativeEight : public Abstract::Patch, public FlashSettings
{
public:
   RelativeEight();

public:
   Sequence* getSequence(const uint8_t& index);
   Sequence* getCurrentSequence();
   Channel* getChannel(const uint8_t& index);

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

   // gates
   void clockTick();
   void clockReset();

private: // things to remeber
   using SequenceArray_ = Remember::RefArray<Sequence, 8>;

private:
   ClockedTempo tempo;
   SequenceArray_ sequences;
   Channel channels[2];
   GateOscilator gateOscilators[2];
};

#endif // RelativeEightH
