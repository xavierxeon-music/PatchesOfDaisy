#include "AudioDeviceOutputRaw.h"

#include "AudioDeviceDriver.h"

AudioDevice::OutputRaw::OutputRaw(Driver* driver, const Channel& channel)
{
   if (!driver)
      return;

   Driver::OutputFunction outputFunction = std::bind(&OutputRaw::process, this, std::placeholders::_1);
   driver->registerOutputFunction(outputFunction, channel);
}

void AudioDevice::OutputRaw::process(OutputBuffer& outputBuffer)
{
   Q_UNUSED(outputBuffer);
}
