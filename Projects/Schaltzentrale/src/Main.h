#ifndef MainH
#define MainH

#include <Abstract/AbstractPatch.h>
#include <Storage/FlashSettings.h>

#include <Remember.h>

#include <Blocks/RandomChain.h>
#include <Sound/GateOscilator.h>
#include <Tools/Range.h>

class Main : public Abstract::Patch, public FlashSettings
{
public:
   Main();

public:
   RandomChain* getChain(uint8_t index);

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

private: // things to remember
   using ChainList_ = Remember::RefArray<RandomChain, 6>;

private:
   TempoControl tempo;
   RandomChain::Collection chainCollection;
   ChainList_ chainList;
   GateOscilator gateOscilator[4];
   Range::Mapper rampMapper;
};

#endif // MainH
