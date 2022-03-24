#include "Include/MidiBridge.h"

#include <QThread>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

static const std::string targetPortName = "Daisy Seed Built In";

MidiBridge::MidiBridge(Remember::Root* root, const Midi::Channel& receiveChannel)
   : root(root)
   , receiveChannel(receiveChannel)
   , output()
   , input()
   , receiveNoteOnFunction(nullptr)
   , receiveNoteOffFunction(nullptr)
   , receiveControllChangeFunction(nullptr)
   , loadFromDaisyFunction(nullptr)
{
}

MidiBridge::~MidiBridge()
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

void MidiBridge::initMidi(bool verbose)
{
   openOutput(verbose);
   openInput(verbose);
}

void MidiBridge::sendNoteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity)
{
   Bytes message;

   message << (Midi::Event::NoteOn | channel);
   message << note.midiValue;
   message << velocity;

   output.sendMessage(&message);
}

void MidiBridge::sendNoteOff(const Midi::Channel& channel, const Note& note)
{
   Bytes message;

   message << (Midi::Event::NoteOff | channel);
   message << note.midiValue;
   message << 127;

   output.sendMessage(&message);
}

void MidiBridge::sendControllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& cotrollerMessage, const uint8_t& value)
{
   Bytes message;

   message << (Midi::Event::ControlChange | channel);
   message << cotrollerMessage;
   message << value;

   output.sendMessage(&message);
}

void MidiBridge::requestLoadFromDaisy()
{
   Bytes message;
   message << (Midi::Event::ControlChange | 0); // control change @ channel 1
   message << Midi::ControllerMessage::RememberRequest;
   message << 0;

   output.sendMessage(&message);
}

void MidiBridge::saveToDaisy()
{
   if (!root)
      return;

   Remember::DataVector data = root->get();
   Bytes dataBase64 = SevenBit::encode(data);

   Bytes message(3);

   message[0] = (Midi::Event::ControlChange | 0); // control change @ channel 1
   message[1] = Midi::ControllerMessage::RememberBlock;

   for (const uint8_t byte : dataBase64)
   {
      message[2] = byte;
      output.sendMessage(&message);
      QThread::msleep(1);
   }

   message[1] = Midi::ControllerMessage::RememberApply;
   message[2] = 0;
   output.sendMessage(&message);
}

void MidiBridge::openOutput(bool verbose)
{
   if (output.isPortOpen())
      return;

   if (verbose)
      qDebug() << "available outputs:";

   uint portNumber = 255;
   for (uint index = 0; index < output.getPortCount(); index++)
   {
      const std::string portName = output.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(portName);

      if (targetPortName != portName)
         continue;

      portNumber = index;
      break;
   }

   if (255 != portNumber)
   {
      output.openPort(portNumber);
      output.setErrorCallback(&MidiBridge::midiError, this);

      qDebug() << "opend midi output port" << portNumber;
   }
}

void MidiBridge::openInput(bool verbose)
{
   if (input.isPortOpen())
      return;

   if (verbose)
      qDebug() << "available inputs:";

   uint portNumber = 255;
   for (uint index = 0; index < input.getPortCount(); index++)
   {
      const std::string portName = input.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(portName);

      if (targetPortName != portName)
         continue;

      portNumber = index;
      break;
   }
   if (255 != portNumber)
   {
      input.openPort(portNumber);

      input.setErrorCallback(&MidiBridge::midiError, nullptr);
      input.setCallback(&MidiBridge::midiReceive, this);
      input.ignoreTypes(false, false, false); // do not ignore anything

      qDebug() << "opend midi input port" << portNumber;
   }
}

void MidiBridge::dataFromDaisy(const Bytes& message)
{
   static Bytes buffer;

   if (message.size() != 3)
      return;

   static const uint8_t noteOnHeader = Midi::Event::NoteOn | receiveChannel;
   static const uint8_t noteOffHeader = Midi::Event::NoteOff | receiveChannel;
   static const uint8_t controlChangeHeader = Midi::Event::ControlChange | receiveChannel;

   if (noteOnHeader == message[0])
   {
      if (receiveNoteOnFunction)
      {
         const Note note = Note::fromMidi(message[1]);
         const Midi::Velocity velocity = message[2];
         receiveNoteOnFunction(note, velocity);
      }
   }
   else if (noteOffHeader == message[0])
   {
      if (receiveNoteOffFunction)
      {
         const Note note = Note::fromMidi(message[1]);
         receiveNoteOffFunction(note);
      }
   }
   else if (controlChangeHeader == message[0])
   {
      if (Midi::ControllerMessage::RememberBlock == message[1])
      {
         buffer << message[2];
      }
      else if (Midi::ControllerMessage::RememberApply == message[1])
      {
         Remember::DataVector data = SevenBit::decode(buffer);
         buffer.clear();

         if (root)
         {
            root->set(data);

            if (loadFromDaisyFunction)
               loadFromDaisyFunction();
         }
      }
      else if (receiveControllChangeFunction)
      {
         const Midi::ControllerMessage controllerMessage = static_cast<Midi::ControllerMessage>(message[1]);
         const uint8_t value = message[2];
         receiveControllChangeFunction(controllerMessage, value);
      }
   }
}

void MidiBridge::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   if (nullptr != userData) // output
      qDebug() << "output" << type << QString::fromStdString(errorText);
   else
      qDebug() << "input" << type << QString::fromStdString(errorText);
}

void MidiBridge::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   MidiBridge* me = reinterpret_cast<MidiBridge*>(userData);
   if (!me)
      return;

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      me->dataFromDaisy(buffer); // may cause threading issues, since callback is not in main thread
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
