#include "ImposterMidiUsbHandler.h"

#include <Midi/MidiCommon.h>

static const QString portName = "Daisy Seed Built In";

Imposter::MidiUsbHandler::MidiUsbHandler()
   : QObject(nullptr)
   , input(this, portName)
   , output(this, portName)
   , eventList()
{
   input.onNoteOn(this, &MidiUsbHandler::noteOn);
   input.onNoteOff(this, &MidiUsbHandler::noteOff);
   input.onControllChange(this, &MidiUsbHandler::controllChange);

   input.open();
   output.open();
}

Imposter::MidiUsbHandler::~MidiUsbHandler()
{
   output.close();
   input.close();
}

void Imposter::MidiUsbHandler::StartReceive()
{
   // do nothing
}

void Imposter::MidiUsbHandler::Listen()
{
   // do nothing
}

bool Imposter::MidiUsbHandler::HasEvents() const
{
   return !eventList.isEmpty();
}

Imposter::MidiEvent Imposter::MidiUsbHandler::PopEvent()
{
   return eventList.takeFirst();
}

void Imposter::MidiUsbHandler::SendMessage(uint8_t* bytes, size_t size)
{
   std::vector<unsigned char> message(size);
   std::memcpy(message.data(), bytes, size);

   output.sendBuffer(message);
}

void Imposter::MidiUsbHandler::noteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity)
{
   MidiEvent event;
   event.type = NoteOn;
   event.channel = channel;
   event.data[0] = note.midiValue;
   event.data[1] = velocity;
   event.sysex_message_len = 0;

   eventList.append(event);
}

void Imposter::MidiUsbHandler::noteOff(const Midi::Channel& channel, const Note& note)
{
   MidiEvent event;
   event.type = NoteOff;
   event.channel = channel;
   event.data[0] = note.midiValue;
   event.data[1] = 0;
   event.sysex_message_len = 0;

   eventList.append(event);
}

void Imposter::MidiUsbHandler::controllChange(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)
{
   MidiEvent event;
   event.type = ControlChange;
   event.channel = channel;
   event.data[0] = controllerMessage;
   event.data[1] = value;
   event.sysex_message_len = 0;

   eventList.append(event);
}
