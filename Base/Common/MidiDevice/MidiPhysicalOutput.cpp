#include <Midi/MidiPhysicalOutput.h>

#include <QDebug>

Midi::Physical::Output::Output(QObject* parent, const QString& portName)
   : RtMidi::Output(parent, portName)
{
}

Midi::Physical::Output::~Output()
{
   Physical::Output::close();
}

void Midi::Physical::Output::open()
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
      output.setErrorCallback(&Physical::Output::midiError);

      qInfo() << "opened midi output port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi output" << portName;
   }
}

void Midi::Physical::Output::close()
{
   if (!output.isPortOpen())
      return;

   output.closePort();
   qInfo() << "closed midi output port";
}

bool Midi::Physical::Output::isOpen() const
{
   return output.isPortOpen();
}
