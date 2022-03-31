#ifndef MidiDeviceHPP
#define MidiDeviceHPP

#include "MidiDevice.h"

template <typename ClassType>
void MidiDevice::onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Note&, const Midi::Velocity&))
{
   NoteOnFunction noteOnFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
   noteOnFunctionList.push_back(noteOnFunction);
}

template <typename ClassType>
void MidiDevice::onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Note&))
{
   NoteOffFunction noteOffFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2);
   noteOffFunctionList.push_back(noteOffFunction);
}

template <typename ClassType>
void MidiDevice::onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Midi::ControllerMessage&, const uint8_t&))
{
   ControllChangeFunction controllChangeFunction = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
   controllChangeFunctionList.push_back(controllChangeFunction);
}

#endif // NOT MidiDeviceHPP
