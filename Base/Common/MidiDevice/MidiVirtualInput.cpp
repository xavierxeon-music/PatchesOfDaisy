#include <Midi/MidiVirtualInput.h>

Midi::Virtual::Input::Input(const QString& inputPortName)
   : Interface::Input()
   , input()
   , inputPortName(inputPortName)
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

   input.openVirtualPort(inputPortName.toStdString());

   input.setErrorCallback(&Virtual::Input::midiError);
   input.setCallback(&Virtual::Input::midiReceive, this);
   input.ignoreTypes(false, false, false); // do not ignore anything

   qInfo() << "opened virtual midi input " << inputPortName;
   isOpen = true;
}

void Midi::Virtual::Input::close()
{
   if (!isOpen)
      return;

   input.closePort();
   qInfo() << "closed virtual midi input" << inputPortName;

   isOpen = false;
}

void Midi::Virtual::Input::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "input" << type << QString::fromStdString(errorText);
}

void Midi::Virtual::Input::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   Input* me = reinterpret_cast<Input*>(userData);
   if (!me)
      return;

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      me->dataFromInput(buffer); // may cause threading issues, since callback is not in main thread

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
