#ifndef AudioDeviceDriverHPP
#define AudioDeviceDriverHPP

#include "AudioDeviceDriver.h"

template <typename ClassType>
void AudioDevice::Driver::registerAudioLoopFunction(ClassType* instance, void (ClassType::*functionPointer)(const float&))
{
   audioLoopFunction = std::bind(functionPointer, instance, std::placeholders::_1);
}

template <typename ClassType>
void AudioDevice::Driver::registerCallbackOverrideFunction(ClassType* instance, void (ClassType::*functionPointer)(const void*, void*, const Frame&))
{
   callbackOverride = std::bind(functionPointer, instance, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
}

#endif // AudioDeviceDriverHPP
