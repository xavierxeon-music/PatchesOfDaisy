#include "AudioDeviceInputsTempo.h"

AudioDevice::InputsTempo::InputsTempo(Driver* driver, const Channel& channelClock, const Channel& channelReset)
   : Tempo()
   , clockInput(driver, channelClock)
   , resetInput(driver, channelReset)
   , tickActive(false)
{
}

void AudioDevice::InputsTempo::advance(const float audioCallackRate)
{
   if (resetInput.isOn())
   {
      clockReset();
      tickActive = false;
   }
   else if (clockInput.isOn())
   {
      if (!tickActive)
         clockTick();
      tickActive = true;
   }
   else
   {
      Tempo::advance(audioCallackRate);
      tickActive = false;
   }
}
