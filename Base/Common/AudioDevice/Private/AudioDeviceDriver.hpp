#ifndef AudioDeviceDriverHPP
#define AudioDeviceDriverHPP

#include "AudioDeviceDriver.h"

template <typename ClassType>
void AudioDevice::Driver::registerAudioLoopFunction(ClassType* instance, void (ClassType::*functionPointer)(const float&))
{
   audioLoopFunction = std::bind(functionPointer, instance, std::placeholders::_1);
}

#endif // AudioDeviceDriverHPP
