#include "AudioDeviceOutputOscilator.h"

#include "AudioDeviceDriver.h"

AudioDevice::OutputOscilator::OutputOscilator(Driver* driver, const Channel& channel, const Standard::Waveform::Shape& waveform)
   : OutputRaw(driver, channel)
   , TableOscilator()
{
   init(Standard::getTable(waveform), driver->getSampleRate());
}

void AudioDevice::OutputOscilator::process(OutputBuffer& outputBuffer)
{
   for (Frame frame = 0; frame < outputBuffer.getNoOfFrames(); frame++)
      outputBuffer[frame] = createSound();
}
