#include <Midi/MidiVirtualInput.h>

#include <QDebug>

Midi::Virtual::Input::Input(QObject* parent, const QString& portName)
   : RtMidi::Input(parent, portName)
   , isOpen(false)
{
}

Midi::Virtual::Input::~Input()
{
   Virtual::Input::close();
}

void Midi::Virtual::Input::open()
{
   if (isOpen)
      return;

   input.openVirtualPort(portName.toStdString());

   input.setErrorCallback(&RtMidi::Input::midiError);
   input.setCallback(&RtMidi::Input::midiReceive, this);
   input.ignoreTypes(false, false, false); // do not ignore anything

   qInfo() << "opened virtual midi input " << portName;
   isOpen = true;
}

void Midi::Virtual::Input::close()
{
   if (!isOpen)
      return;

   input.closePort();
   qInfo() << "closed virtual midi input" << portName;

   isOpen = false;
}

