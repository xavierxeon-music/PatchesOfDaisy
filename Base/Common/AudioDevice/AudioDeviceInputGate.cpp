#include "AudioDeviceInputGate.h"

AudioDevice::InputGate::InputGate()
   : InputRaw()
   , on(false)
   , triggered(false)
{
}

AudioDevice::InputGate::InputGate(Driver* driver, const Channel& channel)
   : InputGate()
{
   activate(driver, channel);
}

bool AudioDevice::InputGate::isOn() const
{
   return on;
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
      if (!on)
         triggered = true;
      on = true;
   }
   else
      on = false;
}
