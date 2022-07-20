#include "Private/MidiRtMidiOutput.h"

#include <QDebug>
#include <QThread>

Midi::RtMidi::Output::Output(QObject* parent, const QString& portName)
   : Base(parent, portName)
   , Interface::Output()
   , output()
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
      const QString rawPortName = QString::fromStdString(testPortName);
      const QString sequencerPortName = PortName::makeNice(rawPortName);
      deviceList << sequencerPortName;
   }

   return deviceList;
}


void Midi::RtMidi::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
   QThread::usleep(1000); // estimate is 1 ms per 3 bytes
}
