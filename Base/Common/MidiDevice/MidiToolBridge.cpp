#include <Midi/MidiToolBridge.h>

#include <QThread>

#include <Tools/SevenBit.h>

Midi::Tool::Bridge::Bridge(Remember::Root* root, Interface::Input* input, Interface::Output* output, const Channel& myChannel, const Channel& remoteChannel)
   : root(root)
   , input(input)
   , output(output)
   , myChannel(myChannel)
   , remoteChannel(remoteChannel)
   , pulledFromRemoteFunction(nullptr)
{
   input->onReceiveControllChange(this, &Bridge::checkRequests);
}

void Midi::Tool::Bridge::requestPullFromRemote()
{
   output->sendControllerChange(remoteChannel, ControllerMessage::RememberRequest, 0);
}

void Midi::Tool::Bridge::pushToRemote()
{
   if (!root)
      return;

   Remember::DataVector data = root->get();
   Bytes dataBase64 = SevenBit::encode(data);

   Bytes message(3);

   for (const uint8_t byte : dataBase64)
   {
      output->sendControllerChange(remoteChannel, ControllerMessage::RememberBlock, byte);
      QThread::msleep(1);
   }

   output->sendControllerChange(remoteChannel, ControllerMessage::RememberApply, 0);
}

void Midi::Tool::Bridge::checkRequests(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)
{
   static Bytes buffer;

   if (channel != myChannel)
      return;

   if (ControllerMessage::RememberBlock == controllerMessage)
   {
      buffer << value;
   }
   else if (ControllerMessage::RememberApply == controllerMessage)
   {
      Remember::DataVector data = SevenBit::decode(buffer);
      buffer.clear();

      if (root)
      {
         root->set(data);

         if (pulledFromRemoteFunction)
            pulledFromRemoteFunction();
      }
   }
}
