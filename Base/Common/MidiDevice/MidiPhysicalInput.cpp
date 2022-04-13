#include <Midi/MidiPhysicalInput.h>

#include <QDebug>

Midi::Physical::Input::Input(QObject* parent, const QString& portName)
   : RtMidi::Input(parent, portName)
{
}

Midi::Physical::Input::~Input()
{
   Physical::Input::close();
}

void Midi::Physical::Input::open()
{
   if (input.isPortOpen())
      return;

   uint portNumber = 255;
   for (uint index = 0; index < input.getPortCount(); index++)
   {
      const QString testPortName = QString::fromStdString(input.getPortName(index));
      if (portName != testPortName)
         continue;

      portNumber = index;
      break;
   }
   if (255 != portNumber)
   {
      input.openPort(portNumber);

      input.setErrorCallback(&RtMidi::Input::midiError);
      input.setCallback(&RtMidi::Input::midiReceive, this);
      input.ignoreTypes(false, false, false); // do not ignore anything

      qInfo() << "opened midi input port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi input" << portName;
   }
}

void Midi::Physical::Input::close()
{
   if (!input.isPortOpen())
      return;

   input.closePort();
   qInfo() << "closed midi input port";
}
