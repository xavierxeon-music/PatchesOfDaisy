#ifndef MidiBridgeH
#define MidiBridgeH

#include "MidiDevice.h"

#include <Remember.h>

// midi connection to a dasiy seed via usb
class MidiBridge : public MidiDevice
{
public:
   using LoadedFromDaisyFunction = std::function<void()>;

public:
   MidiBridge(Remember::Root* root = nullptr, const Midi::Channel& receiveChannel = 0);

public:

   template <typename ClassType>
   void onLoadedFromDaisy(ClassType* instance, void (ClassType::*functionPointer)());

   void requestLoadFromDaisy();
   void saveToDaisy();

private:
   void checkLoadFromDaisy(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value);

private:
   Remember::Root* root;
   const Midi::Channel receiveChannel;

   LoadedFromDaisyFunction loadedFromDaisyFunction;
};

#ifndef MidiBridgeHPP
#include "MidiBridge.hpp"
#endif // NOT MidiBridgeHPP

#endif // NOT MidiBridgeH
