#ifndef AudioDeviceInputsTempoH
#define AudioDeviceInputsTempoH

#include <Music/Tempo.h>

#include <functional>

#include "AudioDeviceInputGate.h"

namespace AudioDevice
{
   class InputsTempo : public Tempo
   {
   public:
      using ClockFunction = std::function<void()>;

   public:
      InputsTempo(Driver* driver, const Channel& channelClock, const Channel& channelReset);

   public:
      void advance(const float audioCallackRate) override;

      template <typename ClassType>
      void onClockTick(ClassType* instance, void (ClassType::*functionPointer)());

      template <typename ClassType>
      void onClockReset(ClassType* instance, void (ClassType::*functionPointer)());

   private:
      AudioDevice::InputGate clockInput;
      AudioDevice::InputGate resetInput;
      bool tickActive;

      QList<ClockFunction> tickFunctionList;
      QList<ClockFunction> resetFunctionList;
   };
} // namespace AudioDevice

#ifndef AudioDeviceInputsTempoHPP
#include "../AudioDeviceInputsTempo.hpp"
#endif // NOT AudioDeviceInputsTempoHPP

#endif // NOT AudioDeviceInputsTempoH
