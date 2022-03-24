#include "ImposterMidiUsbHandler.h"

#include <Midi/MidiCommon.h>

static const std::string portName = "Daisy Seed Built In";

Imposter::MidiUsbHandler::MidiUsbHandler()
   : QObject(nullptr)
   , output()
   , input()
   , eventList()
{
   output.openVirtualPort(portName);
   output.setErrorCallback(&Imposter::MidiUsbHandler::midiError, this);

   input.openVirtualPort(portName);
   input.setErrorCallback(&Imposter::MidiUsbHandler::midiError, nullptr);
   input.setCallback(&Imposter::MidiUsbHandler::midiReceive, this);
   input.ignoreTypes(false, false, false); // do not ignore anything

   qDebug() << "opeend usb midi imposter ports";
}

Imposter::MidiUsbHandler::~MidiUsbHandler()
{
   output.closePort();
   input.closePort();

   qDebug() << "closed usb midi imposter ports";
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

   output.sendMessage(&message);
}

void Imposter::MidiUsbHandler::dataReceived(const Bytes& message)
{
   if (3 != message.size())
      return;

   MidiEvent event;

   Midi::Event midiEventType = static_cast<Midi::Event>(message[0] & 0xF0);
   if (Midi::Event::ControlChange == midiEventType)
      event.type = ControlChange;
   else if (Midi::Event::NoteOn == midiEventType)
      event.type = NoteOn;
   else if (Midi::Event::NoteOff == midiEventType)
      event.type = NoteOff;
   else
      return;

   event.channel = message[0] & 0x0F;
   event.data[0] = message[1];
   event.data[1] = message[2];
   event.sysex_message_len = 0;

   eventList.append(event);
}

void Imposter::MidiUsbHandler::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   if (nullptr != userData) // output
      qDebug() << "output" << type << QString::fromStdString(errorText);
   else
      qDebug() << "input" << type << QString::fromStdString(errorText);
}

void Imposter::MidiUsbHandler::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   static int once = qRegisterMetaType<Bytes>("Bytes");
   Q_UNUSED(once);

   if (!message || !userData)
      return;

   MidiUsbHandler* me = reinterpret_cast<MidiUsbHandler*>(userData);
   if (!me)
      return;

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      MidiUsbHandler::staticMetaObject.invokeMethod(me, "dataReceived", Qt::QueuedConnection, Q_ARG(Bytes, buffer));
      buffer.clear();
   };

   static const uint8_t mask = 0x80;
   for (const uint8_t byte : *message)
   {
      const uint8_t test = byte & mask;
      if (test == mask) // new message start
         maybeProcessBuffer();

      buffer << byte;
   }
   maybeProcessBuffer();
}
