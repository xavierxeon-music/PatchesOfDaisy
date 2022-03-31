#include "MidiDevice.h"

Midi::Device::Device(const std::string portName)
   : Interface()
   , output()
   , input()
   , portName(portName)
{
}

Midi::Device::~Device()
{
   if (output.isPortOpen())
   {
      output.closePort();
      qDebug() << "closed midi output port";
   }
   if (input.isPortOpen())
   {
      input.closePort();
      qDebug() << "closed midi input port";
   }
}

void Midi::Device::initMidi(bool verbose)
{
   openOutput(verbose);
   openInput(verbose);
}

void Midi::Device::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
}

void Midi::Device::dataFromInput(const Bytes& message)
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

void Midi::Device::openOutput(bool verbose)
{
   if (output.isPortOpen())
      return;

   if (verbose)
      qDebug() << "available outputs:";

   uint portNumber = 255;
   for (uint index = 0; index < output.getPortCount(); index++)
   {
      const std::string testPortName = output.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(testPortName);

      if (portName != testPortName)
         continue;

      portNumber = index;
      break;
   }

   if (255 != portNumber)
   {
      output.openPort(portNumber);
      output.setErrorCallback(&Midi::Device::midiError, this);

      qInfo() << "opened midi output port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi output";
   }
}

void Midi::Device::openInput(bool verbose)
{
   if (input.isPortOpen())
      return;

   if (verbose)
      qDebug() << "available inputs:";

   uint portNumber = 255;
   for (uint index = 0; index < input.getPortCount(); index++)
   {
      const std::string testPortName = input.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(testPortName);

      if (portName != testPortName)
         continue;

      portNumber = index;
      break;
   }
   if (255 != portNumber)
   {
      input.openPort(portNumber);

      input.setErrorCallback(&Midi::Device::midiError, nullptr);
      input.setCallback(&Midi::Device::midiReceive, this);
      input.ignoreTypes(true, true, false); // ignore sysex and time

      qInfo() << "opened midi input port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi input";
   }
}

void Midi::Device::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   if (nullptr != userData) // output
      qDebug() << "output" << type << QString::fromStdString(errorText);
   else
      qDebug() << "input" << type << QString::fromStdString(errorText);
}

void Midi::Device::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   Device* me = reinterpret_cast<Device*>(userData);
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
