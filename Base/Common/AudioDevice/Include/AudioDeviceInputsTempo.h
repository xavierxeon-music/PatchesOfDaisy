#ifndef AudioDeviceInputsTempoH
#define AudioDeviceInputsTempoH

#include <Music/Tempo.h>

#include "AudioDeviceInputGate.h"

namespace AudioDevice
{
   class InputsTempo : public Tempo
   {
   public:
      InputsTempo(Driver* driver, const Channel& channelClock, const Channel& channelReset);

   public:
      void advance(const float audioCallackRate) override;

   private:
      AudioDevice::InputGate clockInput;
      AudioDevice::InputGate resetInput;
      bool tickActive;
   };
} // namespace AudioDevice

#endif // AudioDeviceInputsTempoH
