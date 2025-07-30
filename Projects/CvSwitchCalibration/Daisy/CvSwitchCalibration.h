#ifndef CvSwitchCalibrationH
#define CvSwitchCalibrationH

// base
#include <Abstract/AbstractPatch.h>

#include <Blocks/CvSwitch.h>
#include <Tools/Range.h>
#include <Tools/RingBuffer.h>

class CvSwitchCalibration : public Abstract::Patch
{
public:
   CvSwitchCalibration();

public:
   std::string getDeviceName() const;
   void changeDevice();

   const uint8_t& getIndex() const;
   void changeIndex(bool up);

   const float& getVoltage() const;
   const float& getOffset() const;
   const bool& getManualOffset() const;

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

private:
   CvSwitch::StandardDevices::TargetDevice device;
   CvSwitch cvSwitch;
   uint8_t index;

   float voltage;
   RingBuffer<float, 128> offsetBuffer;
   float offset;
   bool manualOffset;
};

#endif // CvSwitchCalibrationH
