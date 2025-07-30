#include "RelativeEight.h"

#include <Abstract/AbstractPage.h>
#include <Tools/Range.h>
#include <Tools/Variable.h>

#include "PageOverview.h"
#include "PageSequence.h"

RelativeEight::RelativeEight()
   : Abstract::Patch("RelativeEight")
   , FlashSettings(this, 11)
   , tempo()
   , sequences(this)
   , channels{Channel(true), Channel(false)}
   , gateOscilators()
{
   load();

   pageManager.addPage(new PageOverview(this, tempo));
   pageManager.addPage(new PageSequence(this));

   for (uint8_t index = 0; index < 8; index++)
      sequences[index]->init();

   clockReset();
}

Sequence* RelativeEight::getSequence(const uint8_t& index)
{
   return sequences[index];
}

Sequence* RelativeEight::getCurrentSequence()
{
   const uint8_t index = channels[1].selection;
   return sequences[index];
}

Channel* RelativeEight::getChannel(const uint8_t& index)
{
   return &(channels[index]);
}

void RelativeEight::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;

   // input
   const Input input = compileInput();
   const ClockedTempo::Event event = tempo.advance(input, daisy.AudioCallbackRate());
   if (ClockedTempo::Event::Reset == event)
      clockReset();
   else if (ClockedTempo::Event::Tick == event)
      clockTick();

   // output
   Output output;
   for (uint8_t channelIndex = 0; channelIndex < 2; channelIndex++)
   {
      Channel* channel = getChannel(channelIndex);
      channel->compile(input);

      if (tempo.isRunningOrFirstTick())
      {
         Sequence* sequence = sequences[channel->selection];
         const uint8_t seqValue = sequence->currentStepValue(tempo);
         if (Sequence::pauseValue == seqValue) // rest
         {
            gateOscilators[channelIndex].setOn(false);
            output.cvs[channelIndex] = channel->rootNote.voltage / 5.0;
         }
         else // play note
         {
            const uint8_t midiValue = channel->rootNote.midiValue + seqValue;
            gateOscilators[channelIndex].setOn(true);
            output.cvs[channelIndex] = Note::fromMidi(midiValue).voltage / 5.0;
         }
      }
      else
      {
         gateOscilators[channelIndex].setOn(false);
         output.cvs[channelIndex] = channel->rootNote.voltage / 5.0;
      }

      // apply gates
      for (size_t chunk = 0; chunk < size; chunk++)
      {
         out[channelIndex][chunk] = gateOscilators[channelIndex].createSound();
      }
   }

   const bool runningPastFirstTick = (Tempo::RunState::Running == tempo.getRunState());
   output.led = runningPastFirstTick;
   output.gate = runningPastFirstTick;
   applyOutput(output);

   // update ui
   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);
}

void RelativeEight::nonAudioLoop()
{
   ensureSettignsValid();
}

void RelativeEight::clockTick()
{
   for (uint8_t index = 0; index < 8; index++)
      sequences[index]->clockTick();
}

void RelativeEight::clockReset()
{
   for (uint8_t index = 0; index < 8; index++)
      sequences[index]->clockReset();
}

// main function

int main(void)
{
   RelativeEight app;
   app.run();
}
