#include "Private/MidiRtMidiInput.h"

#include <Midi/MidiInterfaceOutput.h>

Midi::RtMidi::Input::Input(QObject* parent, const QString& portName)
   : QObject(parent)
   , Interface::Input()
   , input()
   , portName(portName)
{
}

Midi::RtMidi::Input::~Input()
{
}

QStringList Midi::RtMidi::Input::getAvailable()
{
   QStringList deviceList;

   RtMidiIn dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      deviceList << QString::fromStdString(testPortName);
   }

   return deviceList;
}

void Midi::RtMidi::Input::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "input" << type << QString::fromStdString(errorText);
}

void Midi::RtMidi::Input::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   Input* me = reinterpret_cast<Input*>(userData);
   if (!me)
      return;

   static int once = qRegisterMetaType<Bytes>("Bytes");
   Q_UNUSED(once);

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      Input::staticMetaObject.invokeMethod(me, "dataFromInput", Qt::QueuedConnection, Q_ARG(Bytes, buffer));

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

void Midi::RtMidi::Input::dataFromInput(const Bytes& message)
{
   for (Interface::Output* passthrough : passthroughList)
      passthrough->sendBuffer(message);

   Interface::Input::dataFromInput(message);
}
