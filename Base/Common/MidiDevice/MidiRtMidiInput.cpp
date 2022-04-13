#include "Private/MidiRtMidiInput.h"

#include <QDebug>

#include <Midi/MidiInterfaceOutput.h>

Midi::RtMidi::Input::Input(QObject* parent, const QString& portName)
   : Base(parent)
   , Interface::Input()
   , input()
   , portName(PortName::makeRaw(portName))
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
      const QString rawPortName = QString::fromStdString(testPortName);
      const QString sequencerPortName = PortName::makeNice(rawPortName);
      deviceList << sequencerPortName;
   }

   return deviceList;
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
