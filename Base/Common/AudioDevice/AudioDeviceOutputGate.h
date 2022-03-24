#ifndef AudioDeviceOutputGateH
#define AudioDeviceOutputGateH

#include "AudioDeviceOutputRaw.h"

namespace AudioDevice
{
   class OutputGate : public OutputRaw
   {
   public:
      OutputGate(Driver* driver, const Channel& channel);

   public:
      void setActive(bool high);
      void setTrigger();

   protected:
      void process(OutputBuffer& outputBuffer) override;

   private:
      bool active;
      Frame triggerFrame;
      const Frame maxTriggerFrame;
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputGateH
