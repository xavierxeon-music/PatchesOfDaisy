#ifndef MidiToolBridgeH
#define MidiToolBridgeH

#include <Remember.h>

#include <Midi/MidiCommon.h>
#include <Midi/MidiInterfaceInput.h>
#include <Midi/MidiInterfaceOutput.h>

namespace Midi
{
   namespace Tool
   {

      class Bridge
      {
      public:
         using PulledFromRemoteFunction = std::function<void()>;

      public:
         Bridge(Remember::Root* root, Interface::Input* input, Interface::Output* output, const Channel& myChannel = Device::MacBook, const Channel& remoteChannel = Device::Daisy);

      public:
         template <typename ClassType>
         void onPulledFromRemote(ClassType* instance, void (ClassType::*functionPointer)());

         void requestPullFromRemote();
         void pushToRemote();

      private:
         void checkRequests(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value);

      private:
         Remember::Root* root;
         Interface::Output* output;
         const Midi::Channel myChannel;
         const Midi::Channel remoteChannel;

         PulledFromRemoteFunction pulledFromRemoteFunction;
      };
   } // namespace Tool
} // namespace Midi

#ifndef MidiToolBridgeHPP
#include "../../Private/MidiToolBridge.hpp"
#endif // NOT MidiToolBridgeHPP

#endif // NOT MidiToolBridgeH
