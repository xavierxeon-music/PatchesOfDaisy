#include <Midi/MidiToolBridge.h>

#include <Tools/SevenBit.h>

Midi::Tool::Bridge::Bridge(Remember::Root* root, Interface::Input* input, Interface::Output* output, const Channel& myChannel, const Channel& remoteChannel)
   : root(root)
   , output(output)
   , myChannel(myChannel)
   , remoteChannel(remoteChannel)
   , pulledFromRemoteFunction(nullptr)
{
   input->onControllChange(this, &Bridge::checkRequests);
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
   std::string dataBase64 = SevenBit::encode(data);

   for (const char& byte : dataBase64)
      output->sendControllerChange(remoteChannel, ControllerMessage::RememberBlock, byte);

   output->sendControllerChange(remoteChannel, ControllerMessage::RememberApply, 0);
}

void Midi::Tool::Bridge::checkRequests(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)
{
   static std::string buffer;

   if (channel != myChannel)
      return;

   if (ControllerMessage::RememberBlock == controllerMessage)
   {
      buffer += value;
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
