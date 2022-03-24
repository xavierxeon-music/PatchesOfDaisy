#include "AudioDeviceInputCV.h"

AudioDevice::InputCV::InputCV(Driver* driver, const Channel& channel)
   : InputRaw(driver, channel)
   , voltage(0.0)
{
}

const float& AudioDevice::InputCV::getVoltage() const
{
   return voltage;
}

void AudioDevice::InputCV::process(const InputBuffer& inputBuffer)
{
   voltage = 0.0;
   const Frame& noOfFrames = inputBuffer.getNoOfFrames();
   for (Frame frame = 0; frame < noOfFrames; frame++)
      voltage += 10.0 * inputBuffer[frame];

   voltage /= noOfFrames;
}
