#include "AudioDeviceOutputOscilator.h"

#include "AudioDeviceDriver.h"

AudioDevice::OutputOscilator::OutputOscilator()
   : OutputRaw()
   , WaveTable::Oscilator()
   , standardTable()
{
}

AudioDevice::OutputOscilator::OutputOscilator(Driver* driver, const Channel& channel, const Standard::Waveform::Shape& waveform)
   : OutputOscilator()
{
   activate(driver, channel);
   standardTable.setWaveform(waveform);
   init(&standardTable, driver->getSampleRate());
}

void AudioDevice::OutputOscilator::process(OutputBuffer& outputBuffer)
{
   for (Frame frame = 0; frame < outputBuffer.getNoOfFrames(); frame++)
      outputBuffer[frame] = createSound();
}
