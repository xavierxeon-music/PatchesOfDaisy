#include <Midi/MidiVirtualOutput.h>

Midi::Virtual::Output::Output(const QString& outputPortName)
   : Interface::Output()
   , output()
   , outputPortName(outputPortName)
   , isOpen(false)
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

   output.openVirtualPort(outputPortName.toStdString());
   output.setErrorCallback(&Virtual::Output::midiError);

   qInfo() << "opened virtual midi output " << outputPortName;
   isOpen = true;
}

void Midi::Virtual::Output::close()
{
   if (!isOpen)
      return;

   output.closePort();
   qInfo() << "closed virtual midi output" << outputPortName;

   isOpen = false;
}

void Midi::Virtual::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
}

void Midi::Virtual::Output::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "output" << type << QString::fromStdString(errorText);
}
