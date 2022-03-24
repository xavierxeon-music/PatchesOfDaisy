#include "AudioDeviceOutputGate.h"

#include "AudioDeviceDriver.h"

AudioDevice::OutputGate::OutputGate(Driver* driver, const Channel& channel)
   : OutputRaw(driver, channel)
   , active(false)
   , triggerFrame(0)
   , maxTriggerFrame(static_cast<Frame>(driver->getSampleRate() / 100.0)) // 10 milliseconds
{
}

void AudioDevice::OutputGate::setActive(bool high)
{
   active = high;
}

void AudioDevice::OutputGate::setTrigger()
{
   triggerFrame = maxTriggerFrame;
}

void AudioDevice::OutputGate::process(OutputBuffer& outputBuffer)
{
   for (Frame index = 0; index < outputBuffer.getNoOfFrames(); index++)
   {
      if (active || triggerFrame > 0)
         outputBuffer[index] = 0.5; // 5.0V
      else
         outputBuffer[index] = 0.0;

      if (triggerFrame > 0)
         triggerFrame--;
   }
}
