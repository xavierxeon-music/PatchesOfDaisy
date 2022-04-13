#include "Private/MidiRtMidiOutput.h"

#include <QDebug>
#include <QThread>

Midi::RtMidi::Output::Output(QObject* parent, const QString& portName)
   : Base(parent)
   , Interface::Output()
   , output()
   , portName(portName)
{
}

Midi::RtMidi::Output::~Output()
{
}

QStringList Midi::RtMidi::Output::getAvailable()
{
   QStringList deviceList;

   RtMidiOut dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      deviceList << QString::fromStdString(testPortName);
   }

   return deviceList;
}


void Midi::RtMidi::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
   QThread::msleep(1);
}
