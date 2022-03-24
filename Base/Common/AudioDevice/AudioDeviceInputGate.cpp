#include "AudioDeviceInputGate.h"

AudioDevice::InputGate::InputGate(Driver* driver, const Channel& channel)
   : InputRaw(driver, channel)
   , active(false)
   , triggered(false)
{
}

bool AudioDevice::InputGate::isActive() const
{
   return active;
}

bool AudioDevice::InputGate::isTriggered() const
{
   return triggered;
}

void AudioDevice::InputGate::process(const InputBuffer& inputBuffer)
{
   Frame highCount = 0;
   for (Frame index = 0; index < inputBuffer.getNoOfFrames(); index++)
   {
      if (inputBuffer[index] > 0.1) // over 1.0V
         highCount++;
   }

   triggered = false;
   if (2 * highCount >= inputBuffer.getNoOfFrames())
   {
      if (!active)
         triggered = true;
      active = true;
   }
   else
      active = false;
}
