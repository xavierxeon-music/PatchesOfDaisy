#ifndef AudioDeviceOutputGateH
#define AudioDeviceOutputGateH

#include "AudioDeviceOutputRaw.h"

namespace AudioDevice
{
   class OutputGate : public OutputRaw
   {
   public:
      OutputGate();
      OutputGate(Driver* driver, const Channel& channel);

   public:
      void setOn(bool high = true);
      void setOff(bool low = true);
      void setTrigger();
      void activate(Driver* driver, const Channel& channel) override;

   protected:
      void process(OutputBuffer& outputBuffer) override;

   private:
      bool on;
      Frame triggerFrame;
      Frame maxTriggerFrame;
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputGateH
