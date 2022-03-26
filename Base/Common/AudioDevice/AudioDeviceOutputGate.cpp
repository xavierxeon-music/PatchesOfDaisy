#include "AudioDeviceOutputGate.h"

#include "AudioDeviceDriver.h"

AudioDevice::OutputGate::OutputGate()
   : OutputRaw()
   , on(false)
   , triggerFrame(0)
   , maxTriggerFrame(1)
{
}

AudioDevice::OutputGate::OutputGate(Driver* driver, const Channel& channel)
   : OutputGate()
{
   activate(driver, channel);
}

void AudioDevice::OutputGate::setOn(bool high)
{
   on = high;
}

void AudioDevice::OutputGate::setOff(bool low)
{
   on = !low;
}

void AudioDevice::OutputGate::setTrigger()
{
   triggerFrame = maxTriggerFrame;
}

void AudioDevice::OutputGate::activate(Driver* driver, const Channel& channel)
{
   OutputRaw::activate(driver, channel);
   maxTriggerFrame = static_cast<Frame>(driver->getSampleRate() / 100.0); // 10 milliseconds
}

void AudioDevice::OutputGate::process(OutputBuffer& outputBuffer)
{
   for (Frame index = 0; index < outputBuffer.getNoOfFrames(); index++)
   {
      if (on || triggerFrame > 0)
         outputBuffer[index] = 0.5; // 5.0V
      else
         outputBuffer[index] = 0.0;

      if (triggerFrame > 0)
         triggerFrame--;
   }
}
