#ifndef MidiHandlerBaseHPP
#define MidiHandlerBaseHPP

#include <Midi/MidiHandlerBase.h>

#ifndef NON_DAISY_DEVICE

using SystemExclusiveEvent = daisy::SystemExclusiveEvent;

using NoteOnEvent = daisy::NoteOnEvent;
static const auto NoteOnType = daisy::NoteOn;

using NoteOffEvent = daisy::NoteOffEvent;
static const auto NoteOffType = daisy::NoteOff;

using ControlChangeEvent = daisy::ControlChangeEvent;
static const auto ControlChangeType = daisy::ControlChange;

#else

using SystemExclusiveEvent = Imposter::SystemExclusiveEvent;

using NoteOnEvent = Imposter::NoteOnEvent;
static const auto NoteOnType = Imposter::NoteOn;

using NoteOffEvent = Imposter::NoteOffEvent;
static const auto NoteOffType = Imposter::NoteOff;

using ControlChangeEvent = Imposter::ControlChangeEvent;
static const auto ControlChangeType = Imposter::ControlChange;

#endif // NON_DAISY_DEVICE

Midi::Handler::Base::Base(const Channel& receiveChannel, Base* passThroughHandler)
   : Interface::Input()
   , Interface::Output()
   , receiveChannel(receiveChannel)
   , passThroughHandler(passThroughHandler)
{
}

void Midi::Handler::Base::handleMidiMessage(MidiEvent event)
{
   switch (event.type)
   {
      case NoteOnType:
      {
         NoteOnEvent noteOn = event.AsNoteOn();

         if (!noteOnFunctionList.empty() && receiveChannel == noteOn.channel)
         {
            for (const NoteOnFunction& noteOnFunction : noteOnFunctionList)
               noteOnFunction(noteOn.channel, Note::fromMidi(noteOn.note), noteOn.velocity);
         }

         if (passThroughHandler)
            passThroughHandler->sendNoteOn(noteOn.channel, Note::fromMidi(noteOn.note), noteOn.velocity);

         break;
      }
      case NoteOffType:
      {
         NoteOffEvent noteOff = event.AsNoteOff();

         if (!noteOffFunctionList.empty() && receiveChannel == noteOff.channel)
         {
            for (const NoteOffFunction& noteOffFunction : noteOffFunctionList)
               noteOffFunction(noteOff.channel, Note::fromMidi(noteOff.note));
         }

         if (passThroughHandler)
            passThroughHandler->sendNoteOff(noteOff.channel, Note::fromMidi(noteOff.note));

         break;
      }
      case ControlChangeType:
      {
         ControlChangeEvent controlChange = event.AsControlChange();
         ControllerMessage message = static_cast<ControllerMessage>(controlChange.control_number);

         if (!controllerChangeFunctionList.empty() && receiveChannel == controlChange.channel)
         {
            for (const ControllerChangeFunction& controllerChangeFunction : controllerChangeFunctionList)
               controllerChangeFunction(controlChange.channel, message, controlChange.value);
         }

         if (passThroughHandler)
            passThroughHandler->sendControllerChange(controlChange.channel, message, controlChange.value);

         break;
      }
      default:
         break;
   }
}

#endif // MidiHandlerBaseHPP
