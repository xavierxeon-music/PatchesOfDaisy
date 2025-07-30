#ifndef TestThingsHereH
#define TestThingsHereH

// base
#include <Abstract/AbstractPatch.h>

#include <Tools/Range.h>
#include <Tools/RingBuffer.h>

#include "CvSwitch.h"

class TestThingsHere : public Abstract::Patch
{
public:
   TestThingsHere();

public:
   std::string getDeviceName() const;
   void changeDevice();

   const uint8_t& getIndex() const;
   void changeIndex(bool up);

   const float& getVoltage() const;
   const float& getOffset() const;

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

private:
   CvSwitch cvSwitch;
   uint8_t index;
   float voltage;
   RingBuffer<float, 128> offsetBuffer;
   float offset;
};

#endif // TestThingsHereH
