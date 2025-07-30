#include "Main.h"

#include "PageGate.h"
#include "PageOverview.h"
#include "PageRamp.h"

Main::Main()
   : Abstract::Patch("Schaltzentrale")
   , FlashSettings(this, 7)
   , tempo()
   , chainCollection()
   , chainList(this)
   , gateOscilator{}
   , rampMapper(0, 255, 0.0, 5.0)
{
   for (uint8_t i = 0; i < 6; i++)
   {
      if (i < 4)
         chainList[i].setType(RandomChain::Type::AltGate);
      else
         chainList[i].setType(RandomChain::Type::Ramp);
      chainCollection.addChain(&chainList[i]);
   }

   load();

   pageManager.addPage(new PageOverview(this));
   pageManager.addPage(new PageGate(this));
   pageManager.addPage(new PageRamp(this));
}

RandomChain* Main::getChain(uint8_t index)
{
   return &chainList[index];
}

void Main::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;

   const Input input = compileInput();
   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);

   if (input.gates[1])
   {
      chainCollection.clockReset();
      tempo.clockReset();
   }
   else if (input.gates[0])
   {
      chainCollection.clockTick();
      tempo.clockTick();
   }
   else
   {
      tempo.advance(daisy.AudioCallbackRate());
   }

   Output output;
   if (tempo.isRunningOrFirstTick())
   {
      const float& tickPercentage = tempo.getPercentage();
      for (uint8_t i = 0; i < 6; i++)
      {
         RandomChain& chain = chainList[i];
         const float percentage = chain.getCurrentLinkPrecentage(tickPercentage);
         if (i < 4)
         {
            gateOscilator[i].setOn(chain.isOn());

            for (size_t chunk = 0; chunk < size; chunk++)
            {
               const float value = gateOscilator[i].createSound();
               out[i][chunk] = value;
            }
         }
         else
         {
            const uint8_t value = chain.getValue(percentage);
            output.controlVoltages[i - 4] = rampMapper(value);
         }
      }
   }
   else // mute
   {
      for (uint8_t channel = 0; channel < 4; channel++)
      {
         for (size_t chunk = 0; chunk < size; chunk++)
         {
            out[channel][chunk] = 0.0;
         }
      }
      output.controlVoltages[0] = 0.0;
      output.controlVoltages[1] = 0.0;
   }

   applyOutput(output);
}

void Main::nonAudioLoop()
{
   ensureSettignsValid();
}

// main function

int main(void)
{
   Main app;
   app.run();
}
