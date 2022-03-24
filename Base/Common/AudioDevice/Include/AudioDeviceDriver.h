#ifndef AudioDeviceDriverH
#define AudioDeviceDriverH

#include <functional>

#include "../Private/AudioDeviceCommon.h"
#include "../portaudio/install/include/portaudio.h"

namespace AudioDevice
{
   // NO INPUT ON MAC OS? -> start IDE / debugger via terminal (and make sure secure settings allow microphone access for terminal)

   class Driver
   {
   public:
      using AudioLoopFunction = std::function<void(const float& audioCallackRate)>;
      using InputFunction = std::function<void(const InputBuffer& inputBuffer)>;
      using OutputFunction = std::function<void(OutputBuffer& inputBuffer)>;

   public:
      Driver(const QString& deviceName);
      virtual ~Driver();

   public:
      static QStringList listDevices();
      float getSampleRate() const;

      void registerInputFunction(InputFunction inputFunction, const Channel& channel);
      void registerOutputFunction(OutputFunction outputFunction, const Channel& channel);

      template <typename ClassType>
      void registerAudioLoopFunction(ClassType* instance, void (ClassType::*functionPointer)(const float&));

   protected:
      virtual void internalCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer);

   private:
      using InputFunctionMap = QMap<Channel, InputFunction>;
      using OutputFunctionMap = QMap<Channel, OutputFunction>;

   private:
      static int portAudioCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
      void startStream(const PaDeviceIndex& deviceId);

   private:
      static uint useCount;
      const PaDeviceInfo* device;
      PaStream* stream;

      InputFunctionMap inputFunctionMap;
      AudioLoopFunction audioLoopFunction;
      OutputFunctionMap outputFunctionMap;
   };
} // namespace AudioDevice

#include "../Private/AudioDeviceDriver.hpp"

#endif // AudioDeviceDriverH
