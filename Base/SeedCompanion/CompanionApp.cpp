#include "CompanionApp.h"

#include <QFile>
#include <QThread>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

static const std::string targetPortName = "Daisy Seed Built In";

CompanionApp::CompanionApp()
   : Remember::Root()
   , output()
   , input()
{
}

CompanionApp::~CompanionApp()
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

void CompanionApp::loadFromFile(const QString& fileName)
{
   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = file.readAll();
   file.close();

   const uint64_t dataSize = content.size();

   Remember::DataVector data(dataSize);
   std::memcpy(&data[0], content.constData(), dataSize);
   set(data);
}

void CompanionApp::requestLoadFromDaisy()
{
   delayedOpenOutput();
   delayedOpenInput();

   Bytes message;
   message << (Midi::Event::ControlChange | 0); // control change @ channel 1
   message << Midi::ControllerMessage::RememberRequest;
   message << 0;

   output.sendMessage(&message);
}

void CompanionApp::loadedFromDaisy()
{
}

void CompanionApp::saveToFile(const QString& fileName)
{
   Remember::DataVector data = get();
   const uint64_t dataSize = data.size();

   QFile file(fileName);
   if (file.open(QIODevice::WriteOnly))
   {
      file.write((const char*)(&data[0]), dataSize);
      file.close();
   }
}

void CompanionApp::saveToDaisy()
{
   delayedOpenOutput();

   Remember::DataVector data = get();
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

void CompanionApp::delayedOpenOutput(bool verbose)
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
      output.setErrorCallback(&CompanionApp::midiError, this);

      qDebug() << "opend midi output port" << portNumber;
   }
}

void CompanionApp::delayedOpenInput(bool verbose)
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

      input.setErrorCallback(&CompanionApp::midiError, nullptr);
      input.setCallback(&CompanionApp::midiReceive, this);
      input.ignoreTypes(false, false, false); // do not ignore anything

      qDebug() << "opend midi input port" << portNumber;
   }
}

void CompanionApp::dataFromDaisy(const Bytes& message)
{
   static Bytes buffer;

   if (message.size() != 3)
      return;

   static const uint8_t controlChangeHeader = Midi::Event::ControlChange | 0; // control change @ channel 1
   if (controlChangeHeader != message[0])
      return;

   if (Midi::ControllerMessage::RememberBlock == message[1])
   {
      buffer << message[2];
   }
   else if (Midi::ControllerMessage::RememberApply == message[1])
   {
      Remember::DataVector data = SevenBit::decode(buffer);
      buffer.clear();

      set(data);

      loadedFromDaisy();
   }
}

void CompanionApp::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   if (nullptr != userData) // output
      qDebug() << "output" << type << QString::fromStdString(errorText);
   else
      qDebug() << "input" << type << QString::fromStdString(errorText);
}

void CompanionApp::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   CompanionApp* me = reinterpret_cast<CompanionApp*>(userData);
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
