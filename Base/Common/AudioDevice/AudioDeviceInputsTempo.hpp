#ifndef AudioDeviceInputsTempoHPP
#define AudioDeviceInputsTempoHPP

#include "Include/AudioDeviceInputsTempo.h"

template <typename ClassType>
void AudioDevice::InputsTempo::onClockTick(ClassType* instance, void (ClassType::*functionPointer)())
{
   ClockFunction clockFunction = std::bind(functionPointer, instance);
   tickFunctionList.append(clockFunction);
}

template <typename ClassType>
void AudioDevice::InputsTempo::onClockReset(ClassType* instance, void (ClassType::*functionPointer)())
{
   ClockFunction clockFunction = std::bind(functionPointer, instance);
   resetFunctionList.append(clockFunction);
}

#endif // NOT AudioDeviceInputsTempoHPP
