#include "AudioDeviceInputsTempo.h"

AudioDevice::InputsTempo::InputsTempo(Driver* driver, const Channel& channelClock, const Channel& channelReset)
   : Tempo()
   , clockInput(driver, channelClock)
   , resetInput(driver, channelReset)
   , tickActive(false)
   , tickFunctionList()
   , resetFunctionList()
{
}

void AudioDevice::InputsTempo::advance(const float callackRate)
{
   if (resetInput.isOn())
   {
      clockReset();
      for (ClockFunction& clockFunction : resetFunctionList)
         clockFunction();

      tickActive = false;
   }
   else if (clockInput.isOn())
   {
      if (!tickActive)
      {
         clockTick();
         for (ClockFunction& clockFunction : tickFunctionList)
            clockFunction();
      }
      tickActive = true;
   }
   else
   {
      Tempo::advance(callackRate);
      tickActive = false;
   }
}
