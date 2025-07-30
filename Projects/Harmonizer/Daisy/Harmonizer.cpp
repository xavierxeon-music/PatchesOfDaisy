#include "Harmonizer.h"

#include <Abstract/AbstractOscilator.h>
#include <Tools/Variable.h>

#include "PageOverview.h"

Harmonizer::Harmonizer()
   : Abstract::Patch("Harmonizer")
   , FlashSettings(this, 30)
   , filters{}
   , notes{}
   , revoice{}
   , numberOfVoices(this, 16)
{
   load();

   pageManager.addPage(new PageOverview(this));

   const float sampleRate = daisy.AudioSampleRate();
   for (uint8_t channel = 0; channel < 2; channel++)
   {
      filters[channel].init(sampleRate);
      revoice[channel].setNumberOfVoices(numberOfVoices);
      revoice[channel].setSampleRate(sampleRate);
   }
}

const Notes& Harmonizer::getNotes() const
{
   return notes;
}

uint8_t Harmonizer::getNumberOfVoces() const
{
   return numberOfVoices;
}

void Harmonizer::changeNumbertOfVoices(bool more)
{
   static const std::vector<uint8_t> voiceValues = {0, 1, 2, 4, 8, 16, 32};
   Variable::Enum<uint8_t> var(numberOfVoices, voiceValues, true);
   var.change(more);

   setUnsynced();

   for (uint8_t channel = 0; channel < 2; channel++)
   {
      revoice[channel].setNumberOfVoices(numberOfVoices);
   }
}

void Harmonizer::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   const Input input = compileInput();
   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);

   for (uint8_t index = 0; index < 4; index++)
   {
      notes[index] = Note::fromVoltage(input.controlVoltages[index]);
   }

   const float fResonance = 1.0;
   for (uint8_t channel = 0; channel < 2; channel++)
   {
      filters[channel].update(fResonance, notes);

      for (size_t chunk = 0; chunk < size; chunk++)
      {
         // resonance
         {
            const float inputResonance = in[channel + 0][chunk];
            const float outputResonance = filters[channel].changeSound(inputResonance);
            out[channel + 0][chunk] = outputResonance;
         }

         // revoice
         {
            const float inputRevoice = in[channel + 2][chunk];
            const float outputRevoice = revoice[channel].changeSound(inputRevoice);
            out[channel + 2][chunk] = outputRevoice;
         }
      }
   }
}

void Harmonizer::nonAudioLoop()
{
   ensureSettignsValid();
}

// main function

int main(void)
{
   Harmonizer app;
   app.run();
}
