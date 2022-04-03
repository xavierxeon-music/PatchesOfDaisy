#ifndef MidiToolBridgeHPP
#define MidiToolBridgeHPP

#include <Midi/MidiToolBridge.h>

template <typename ClassType>
void Midi::Tool::Bridge::onPulledFromRemote(ClassType* instance, void (ClassType::*functionPointer)())
{
   pulledFromRemoteFunction = std::bind(functionPointer, instance);
}

#endif // NOT MidiToolBridgeHPP
