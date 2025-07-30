#include "Main.h"

#include <Abstract/AbstractPage.h>
#include <Tools/Range.h>
#include <Tools/Variable.h>

#include "PageGates.h"
#include "PageOverview.h"
#include "PageRhythm.h"

Main::Main()
   : Abstract::Patch("BeatMaster")
   , FlashSettings(this, 10)
   , Midi::Handler::Internal(&daisy, nullptr)
   , banks(this)
   , channelIndex(this, 0)
   , gates(this)
   , tempo()
   , bankIndex(0)
   , midiState()
   , gateOscilators()
{
   load();

   for (uint8_t index = 0; index < banks.getSize(); index++)
      banks[index].init();

   for (uint8_t index = 0; index < gates.getSize(); index++)
      gates[index].init();

   pageManager.addPage(new PageOverview(this, tempo));
   pageManager.addPage(new PageRhythm(this));
   pageManager.addPage(new PageGates(this, tempo));

   clockReset();
}

uint8_t Main::getCurrentChannel() const
{
   return channelIndex;
}

void Main::changeCurrentChannel(bool up)
{
   Variable::Integer<uint8_t> var(channelIndex, 0, 15, true);
   var.change(up);

   Remember::Root::setUnsynced();
}

const Main::BankIndex& Main::getBankIndex() const
{
   return bankIndex;
}

RhythmBank& Main::getBank(const uint8_t& bankIndex)
{
   return banks[bankIndex];
}

Gate& Main::getGate(const uint8_t& gateIndex)
{
   return gates[gateIndex];
}

void Main::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;

   // input
   const ::Input input = compileInput();
   const ClockedTempo::Event event = tempo.advance(input, daisy.AudioCallbackRate());
   if (ClockedTempo::Event::Reset == event)
      clockReset();
   else if (ClockedTempo::Event::Tick == event)
      clockTick();

   for (uint8_t index = 0; index < gates.getSize(); index++)
   {
      const bool gateActive = (Gate::Stage::High == gates[index].getCurrentStage());
      bankIndex.set(index, (input.controlVoltages[index] >= 0.5));

      if (tempo.isRunningOrFirstTick())
         gateOscilators[index].setOn(gateActive);
      else
         gateOscilators[index].setOn(false);

      if (gates[index].getApplyToKnobs() && gateActive)
         bankIndex.set(index, true);
   }

   if (tempo.isRunningOrFirstTick())
   {
      ChannelMap onMap;
      ChannelMap offMap;

      RhythmBank& bank = banks[bankIndex];
      for (uint8_t midiIndex = 0; midiIndex < 16; midiIndex++)
      {
         Rhythm& rhythm = bank.getRhythm(midiIndex);
         const float velocity = rhythm.currentVelocity();

         if (velocity == midiState[midiIndex]) // nothing has changed
            continue;

         // state has changed
         if (0 == midiState[midiIndex]) // was off -> turn on
            onMap[midiIndex] = velocity;
         else // was on -> turn off
            offMap[midiIndex] = 0;

         midiState[midiIndex] = velocity;
      }

      sendMidi(offMap, false);
      sendMidi(onMap, true);
   }

   for (uint8_t audioChannel = 0; audioChannel < 4; audioChannel++)
   {
      for (size_t chunk = 0; chunk < size; chunk++)
      {
         out[audioChannel][chunk] = gateOscilators[audioChannel].createSound();
      }
   }

   // process output
   ::Output output;
   for (uint8_t index = 0; index < 2; index++)
   {
      // TODO: graphs
   }

   const bool runningPastFirstTick = (Tempo::RunState::Running == tempo.getRunState());
   output.led = runningPastFirstTick;
   output.gate = runningPastFirstTick;
   applyOutput(output);

   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);
}

void Main::nonAudioLoop()
{
   ensureSettignsValid();
}

void Main::clockTick()
{
   for (uint8_t index = 0; index < gates.getSize(); index++)
      gates[index].clockTick();

   for (uint8_t index = 0; index < banks.getSize(); index++)
      banks[index].clockTick();
}

void Main::clockReset()
{
   for (uint8_t index = 0; index < gates.getSize(); index++)
      gates[index].clockReset();

   ChannelMap offMap;

   for (uint8_t index = 0; index < banks.getSize(); index++)
   {
      if (midiState[index])
         offMap[index] = 0;

      midiState[index] = 0;
      banks[index].clockReset();
   }
   sendMidi(offMap, false);

}

void Main::sendMidi(const ChannelMap channelMap, bool on)
{
   // clang-format off
   static const ChannelNodeMap notes =
   {
      {0, Note::fromMidi(36)},
      {1, Note::fromMidi(37)},
      {2, Note::fromMidi(38)},
      {3, Note::fromMidi(39)},
      
      {4, Note::fromMidi(40)},
      {5, Note::fromMidi(41)},
      {6, Note::fromMidi(42)},
      {7, Note::fromMidi(43)},
      
      {8, Note::fromMidi(44)},
      {9, Note::fromMidi(45)},
      {10, Note::fromMidi(46)},
      {11, Note::fromMidi(47)},
      
      {12, Note::fromMidi(48)},
      {13, Note::fromMidi(49)},
      {14, Note::fromMidi(50)},
      {15, Note::fromMidi(51)}
   };
   // clang-format on

   if (channelMap.empty())
      return;

   for (ChannelMap::const_iterator it = channelMap.cbegin(); it != channelMap.cend(); it++)
   {
      const Note note = notes.at(it->first);
      //qDebug() << (on ? "MIDI ON" : "MIDI OFF") << QString::fromStdString(note.name) << it->second;
      if (on)
         sendNoteOn(0, note, it->second);
      else
         sendNoteOff(0, note);
   }
}

// main function

int main(void)
{
   Main app;
   app.run();
}
