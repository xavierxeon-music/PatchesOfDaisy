#include <Midi/MidiDeviceOutput.h>

#include <QDebug>

Midi::Device::Output::Output(QObject* parent, const QString& portName)
   : RtMidi::Output(parent, portName)
{
}

Midi::Device::Output::~Output()
{
   Device::Output::close();
}

void Midi::Device::Output::open()
{
   if (output.isPortOpen())
      return;

   uint portNumber = 255;
   for (uint index = 0; index < output.getPortCount(); index++)
   {
      const QString testPortName = QString::fromStdString(output.getPortName(index));
      if (portName != testPortName)
         continue;

      portNumber = index;
      break;
   }

   if (255 != portNumber)
   {
      output.openPort(portNumber);
      output.setErrorCallback(&Device::Output::midiError);

      qInfo() << "opened midi output port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi output";
   }
}

void Midi::Device::Output::close()
{
   if (!output.isPortOpen())
      return;

   output.closePort();
   qInfo() << "closed midi output port";
}
