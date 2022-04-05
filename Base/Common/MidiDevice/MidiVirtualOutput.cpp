#include <Midi/MidiVirtualOutput.h>

#include <QDebug>

Midi::Virtual::Output::Output(QObject* parent, const QString& portName)
   : RtMidi::Output(parent, portName)
{
}

Midi::Virtual::Output::~Output()
{
   Virtual::Output::close();
}

void Midi::Virtual::Output::open()
{
   if (isOpen)
      return;

   output.openVirtualPort(portName.toStdString());
   output.setErrorCallback(&Virtual::Output::midiError);

   qInfo() << "opened virtual midi output " << portName;
   isOpen = true;
}

void Midi::Virtual::Output::close()
{
   if (!isOpen)
      return;

   output.closePort();
   qInfo() << "closed virtual midi output" << portName;

   isOpen = false;
}

