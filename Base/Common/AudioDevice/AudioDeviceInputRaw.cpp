#include "AudioDeviceInputRaw.h"

#include "AudioDeviceDriver.h"

AudioDevice::InputRaw::InputRaw(Driver* driver, const Channel& channel)
{
   if (!driver)
      return;

   Driver::InputFunction inputFunction = std::bind(&InputRaw::process, this, std::placeholders::_1);
   driver->registerInputFunction(inputFunction, channel);
}

void AudioDevice::InputRaw::process(const InputBuffer& inputBuffer)
{
   Q_UNUSED(inputBuffer);
}
