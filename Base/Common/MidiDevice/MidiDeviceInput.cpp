#include <Midi/MidiDeviceInput.h>

#include <QDebug>

Midi::Device::Input::Input(QObject* parent, const QString& portName)
   : RtMidi::Input(parent, portName)
{
}

Midi::Device::Input::~Input()
{
   Device::Input::close();
}

void Midi::Device::Input::open()
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
      qWarning() << "unable to open midi input";
   }
}

void Midi::Device::Input::close()
{
   if (!input.isPortOpen())
      return;

   input.closePort();
   qInfo() << "closed midi input port";
}
