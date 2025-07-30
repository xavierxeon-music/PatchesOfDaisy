#include "WaveBlender.h"

#include <Sound/WaveTable.h>
#include <Tools/Counter.h>

#include "PageChannel.h"

WaveBlender::WaveBlender()
   : Abstract::Patch("WaveBlender")
   , FlashSettings(this, 7)
   , randomWalkTables()
   , oscilators{}
   , tables(this)
{
   load();

   const float sampleRate = daisy.AudioSampleRate();

   for (uint8_t channel = 0; channel < 4; channel++)
   {
      CustomTable& customTable = tables[channel];
      customTable.init(&randomWalkTables);

      oscilators[channel].init(&customTable, sampleRate);
      oscilators[channel].setFrequency(200.0);
      oscilators[channel].setAmplitude(1.0);

      pageManager.addPage(new PageChannel(&customTable, &oscilators[channel], channel));
   }
}

void WaveBlender::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;

   const Input input = compileInput();
   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);

   for (uint8_t channel = 0; channel < 4; channel++)
   {
      const float frequency = tables[channel].setCvAndGetFrequency(input.controlVoltages);
      oscilators[channel].setFrequency(frequency);

      for (size_t chunk = 0; chunk < size; chunk++)
      {
         const float value = oscilators[channel].createSound();
         out[channel][chunk] = value;
      }
   }
}

void WaveBlender::nonAudioLoop()
{
   static Counter ledCounter(100);
   daisy.seed.SetLed(0 == ledCounter.valueAndNext());

   ensureSettignsValid();
}

// main function

int main(void)
{
   WaveBlender app;
   app.run();
}
