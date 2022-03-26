#ifndef AudioDeviceInputGateH
#define AudioDeviceInputGateH

#include "AudioDeviceInputRaw.h"

namespace AudioDevice
{
   class InputGate : public InputRaw
   {
   public:
      InputGate();
      InputGate(Driver* driver, const Channel& channel);

   public:
      bool isOn() const;
      bool isTriggered() const;

   private:
      void process(const InputBuffer& inputBuffer) override;

   private:
      bool on;
      bool triggered;
   };
} // namespace AudioDevice

#endif // AudioDeviceInputGateH
