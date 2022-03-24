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
   : receiveChannel(receiveChannel)
   , passThroughHandler(passThroughHandler)
   , receiveNoteOnFunction(nullptr)
   , receiveNoteOffFunction(nullptr)
   , receiveControllChangeFunction(nullptr)
{
}

void Midi::Handler::Base::initMidi()
{
   // do nothing
}

void Midi::Handler::Base::sendNoteOn(const Channel& channel, const Note& note, const Velocity& velocity)
{
   //qDebug() << "NOTE ON" << QString::fromStdString(note.name);

   Bytes buffer;

   buffer << (Event::NoteOn | channel);
   buffer << note.midiValue;
   buffer << velocity;

   sendBuffer(buffer);
}

void Midi::Handler::Base::sendNoteOff(const Channel& channel, const Note& note)
{
   //qDebug() << "NOTE OFF" << QString::fromStdString(note.name);

   Bytes buffer;

   buffer << (Event::NoteOff | channel);
   buffer << note.midiValue;
   buffer << 127;

   sendBuffer(buffer);
}

void Midi::Handler::Base::sendControllerChange(const Channel& channel, const ControllerMessage& cotrollerMessage, const uint8_t& value)
{
   Bytes buffer;

   buffer << (Event::ControlChange | channel);
   buffer << cotrollerMessage;
   buffer << value;

   sendBuffer(buffer);
}

template <typename ClassType>
void Midi::Handler::Base::onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Note&, const Velocity&))
{
   receiveNoteOnFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2);
}

template <typename ClassType>
void Midi::Handler::Base::onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Note&))
{
   receiveNoteOffFunction = std::bind(functionPointer, instance, std::placeholders::_1);
}

template <typename ClassType>
void Midi::Handler::Base::onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const ControllerMessage&, const uint8_t&))
{
   receiveControllChangeFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2);
}

void Midi::Handler::Base::handleMidiMessage(MidiEvent event)
{
   switch (event.type)
   {
      case NoteOnType:
      {
         NoteOnEvent noteOn = event.AsNoteOn();

         if (receiveNoteOnFunction && receiveChannel == noteOn.channel)
            receiveNoteOnFunction(Note::fromMidi(noteOn.note), noteOn.velocity);

         if (passThroughHandler)
            passThroughHandler->sendNoteOn(noteOn.channel, Note::fromMidi(noteOn.note), noteOn.velocity);

         break;
      }
      case NoteOffType:
      {
         NoteOffEvent noteOff = event.AsNoteOff();

         if (receiveNoteOffFunction && receiveChannel == noteOff.channel)
            receiveNoteOffFunction(Note::fromMidi(noteOff.note));

         if (passThroughHandler)
            passThroughHandler->sendNoteOff(noteOff.channel, Note::fromMidi(noteOff.note));

         break;
      }
      case ControlChangeType:
      {
         ControlChangeEvent controlChange = event.AsControlChange();
         ControllerMessage message = static_cast<ControllerMessage>(controlChange.control_number);

         if (receiveControllChangeFunction && receiveChannel == controlChange.channel)
            receiveControllChangeFunction(message, controlChange.value);

         if (passThroughHandler)
            passThroughHandler->sendControllerChange(controlChange.channel, message, controlChange.value);

         break;
      }
      default:
         break;
   }
}

#endif // MidiHandlerBaseHPP
