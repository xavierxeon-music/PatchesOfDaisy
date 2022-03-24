#ifndef AudioDeviceInputGateH
#define AudioDeviceInputGateH

#include "AudioDeviceInputRaw.h"

namespace AudioDevice
{
   class InputGate : public InputRaw
   {
   public:
      InputGate(Driver* driver, const Channel& channel);

   public:
      bool isActive() const;
      bool isTriggered() const;

   private:
      void process(const InputBuffer& inputBuffer) override;

   private:
      bool active;
      bool triggered;
   };
} // namespace AudioDevice

#endif // AudioDeviceInputGateH
