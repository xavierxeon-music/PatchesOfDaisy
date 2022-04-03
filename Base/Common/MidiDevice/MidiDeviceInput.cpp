#include <Midi/MidiDeviceInput.h>

#include <Midi/MidiInterfaceOutput.h>

Midi::Device::Input::Input(const QString& inputPortName)
   : Interface::Input()
   , input()
   , inputPortName(inputPortName.toStdString())
{
}

Midi::Device::Input::~Input()
{
   if (input.isPortOpen())
   {
      input.closePort();
      qInfo() << "closed midi input port";
   }
}

void Midi::Device::Input::initMidi(bool verbose)
{
   openInput(verbose);
}

void Midi::Device::Input::dataFromInput(const Bytes& message)
{
   if (message.size() != 3)
      return;

   const Midi::Channel channel = message[0] & 0x0F;
   if (Midi::Event::NoteOn == (message[0] & 0xF0))
   {
      const Note note = Note::fromMidi(message[1]);
      const Midi::Velocity velocity = message[2];

      for (const NoteOnFunction& noteOnFunction : noteOnFunctionList)
         noteOnFunction(channel, note, velocity);
   }
   else if (Midi::Event::NoteOff == (message[0] & 0xF0))
   {
      const Note note = Note::fromMidi(message[1]);

      for (const NoteOffFunction& noteOffFunction : noteOffFunctionList)
         noteOffFunction(channel, note);
   }
   else if (Midi::Event::ControlChange == (message[0] & 0xF0))
   {
      const Midi::ControllerMessage controllerMessage = static_cast<Midi::ControllerMessage>(message[1]);
      const uint8_t value = message[2];

      for (const ControllChangeFunction& controllChangeFunction : controllChangeFunctionList)
         controllChangeFunction(channel, controllerMessage, value);
   }
}

void Midi::Device::Input::openInput(bool verbose)
{
   if (input.isPortOpen())
      return;

   if (verbose)
      qInfo() << "available inputs:";

   uint portNumber = 255;
   for (uint index = 0; index < input.getPortCount(); index++)
   {
      const std::string testPortName = input.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(testPortName);

      if (inputPortName != testPortName)
         continue;

      portNumber = index;
      break;
   }
   if (255 != portNumber)
   {
      input.openPort(portNumber);

      input.setErrorCallback(&Midi::Device::Input::midiError);
      input.setCallback(&Midi::Device::Input::midiReceive, this);
      input.ignoreTypes(false, false, false); // do not ignore anything

      qInfo() << "opened midi input port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi input";
   }
}

void Midi::Device::Input::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "input" << type << QString::fromStdString(errorText);
}

void Midi::Device::Input::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
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

      for (Interface::Output* passthrough : me->passthroughList)
         passthrough->sendBuffer(buffer);

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
