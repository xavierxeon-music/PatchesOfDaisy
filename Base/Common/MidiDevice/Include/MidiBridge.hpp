#ifndef MidiBridgeHPP
#define MidiBridgeHPP

#include "MidiBridge.h"

template <typename ClassType>
void MidiBridge::onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Note&, const Midi::Velocity&))
{
   receiveNoteOnFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2);
}

template <typename ClassType>
void MidiBridge::onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Note&))
{
   receiveNoteOffFunction = std::bind(functionPointer, instance, std::placeholders::_1);
}

template <typename ClassType>
void MidiBridge::onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const Midi::ControllerMessage&, const uint8_t&))
{
   receiveControllChangeFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2);
}

template <typename ClassType>
void MidiBridge::onLoadedFromDaisy(ClassType* instance, void (ClassType::*functionPointer)())
{
   loadFromDaisyFunction = std::bind(functionPointer, instance);
}

#endif // NOT MidiBridgeHPP
