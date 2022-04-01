#include "Include/MidiBridge.h"

#include <QThread>

#include <Midi/MidiCommon.h>
#include <Tools/SevenBit.h>

static const QString targetPortName = "Daisy Seed Built In";

Midi::Bridge::Bridge(Remember::Root* root, const Midi::Channel& receiveChannel)
   : Device(targetPortName, targetPortName)
   , root(root)
   , receiveChannel(receiveChannel)
   , loadedFromDaisyFunction(nullptr)
{
   onReceiveControllChange(this, &Midi::Bridge::checkLoadFromDaisy);
}

void Midi::Bridge::requestLoadFromDaisy()
{
   Bytes message;
   message << (Midi::Event::ControlChange | 0); // control change @ channel 1
   message << Midi::ControllerMessage::RememberRequest;
   message << 0;

   output.sendMessage(&message);
}

void Midi::Bridge::saveToDaisy()
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

void Midi::Bridge::checkLoadFromDaisy(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)
{
   static Bytes buffer;

   if (channel != receiveChannel)
      return;

   if (Midi::ControllerMessage::RememberBlock == controllerMessage)
   {
      buffer << value;
   }
   else if (Midi::ControllerMessage::RememberApply == controllerMessage)
   {
      Remember::DataVector data = SevenBit::decode(buffer);
      buffer.clear();

      if (root)
      {
         root->set(data);

         if (loadedFromDaisyFunction)
            loadedFromDaisyFunction();
      }
   }
}
