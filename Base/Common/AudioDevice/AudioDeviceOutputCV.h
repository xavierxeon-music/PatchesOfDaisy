#ifndef AudioDeviceOutputCVH
#define AudioDeviceOutputCVH

#include "AudioDeviceOutputRaw.h"

namespace AudioDevice
{
   class OutputCV : public OutputRaw
   {
   public:
      OutputCV(Driver* driver, const Channel& channel);

   public:
      // valid voltages range from -10V to +10V, will be clamped
      void setVoltage(const float& newVoltage);

   protected:
      void process(OutputBuffer& outputBuffer) override;

   private:
      float value;
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputCVH
