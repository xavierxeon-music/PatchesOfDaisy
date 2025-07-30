#ifndef MainH
#define MainH

// base
#include <Abstract/AbstractPatch.h>
#include <Storage/FlashSettings.h>

#include <map>

#include <Midi/MidiHandlerInternal.h>
#include <Music/ClockedTempo.h>
#include <Remember.h>
#include <Sound/GateOscilator.h>
#include <Tools/BoolField.h>

#include "Gate.h"
#include "RhythmBank.h"

class Main : public Abstract::Patch, public FlashSettings, public Midi::Handler::Internal
{
public:
   using BankIndex = BoolField<uint8_t>;

public:
   Main();

public:
   uint8_t getCurrentChannel() const;
   void changeCurrentChannel(bool up);

   const BankIndex& getBankIndex() const;
   RhythmBank& getBank(const uint8_t& bankIndex);

   Gate& getGate(const uint8_t& gateIndex);

private:
   using ChannelId = uint8_t;
   using Velocity = uint8_t;
   using ChannelMap = std::map<ChannelId, Velocity>;
   using ChannelNodeMap = std::map<ChannelId, Note>;

protected: // things to remeber
   using RhythmBankArray_ = Remember::RefArray<RhythmBank, 16>;
   using Channel_ = Remember::Value<uint8_t>;
   using GateArray_ = Remember::RefArray<Gate, 4>;

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

   // gates
   void clockTick();
   void clockReset();

   void sendMidi(const ChannelMap channelMap, bool on);

private:
   RhythmBankArray_ banks;
   Channel_ channelIndex;
   GateArray_ gates;

   ClockedTempo tempo;
   BankIndex bankIndex; // from cv / knobs

   uint8_t midiState[16];

   GateOscilator gateOscilators[4];
};

#endif // MainH
