#include "AudioChannelOutput.h"

AudioChannel::Output::Output()
   : Abstract()
{
}

void AudioChannel::Output::process(float* data, const uint64_t& blockSize)
{
   Buffer values(blockSize, 0.0);
   for (uint64_t index = 0; index < blockSize; index++)
      values[index] = data[index];

   updatePixmap(values);
}
