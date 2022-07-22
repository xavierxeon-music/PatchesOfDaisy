#ifndef AudioDeviceDriverH
#define AudioDeviceDriverH

#include <functional>

#include "../Private/AudioDeviceCommon.h"
#include "../portaudio/install/include/portaudio.h"

namespace AudioDevice
{
   // A sound interface
   // Both Gate and CV poerts (both inputs and outputs) work only with a DC coupled interface!
   class Driver final
   {      
   public:
      using CallbackOverrideFunction = std::function<void(const void* inputBuffer, void* outputBuffer, const Frame& framesPerBuffer)>;
      using AudioLoopFunction = std::function<void(const float& audioCallbackRate)>;
      using InputFunction = std::function<void(const InputBuffer& inputBuffer)>;
      using OutputFunction = std::function<void(OutputBuffer& outputBuffer)>;

      struct DeviceInfo
      {
         QString name;
         Common::SampleRate::BoolMap supportedSampleRates;
         float defaultSampleRate;

         DeviceInfo(const QString& name, const float& defaultSampleRate);

         using Map = QMap<PaDeviceIndex, DeviceInfo>;
      };

   public:
      // leave deviceName empty for default device
      Driver(const QString& deviceName = QString(), const float& sampleRate = Common::SampleRate::Default, const Frame& framesPerBuffer = 0);
      virtual ~Driver();

   public:
      static DeviceInfo::Map listDevices();
      const float& getSampleRate() const;
      const int& getMaxInputChannels() const;
      const int& getMaxOutputChannels() const;

      void registerInputFunction(InputFunction inputFunction, const Channel& channel);
      void registerOutputFunction(OutputFunction outputFunction, const Channel& channel);

      template <typename ClassType>
      void registerAudioLoopFunction(ClassType* instance, void (ClassType::*functionPointer)(const float&));

      template <typename ClassType>
      void registerCallbackOverrideFunction(ClassType* instance, void (ClassType::*functionPointer)(const void*, void*, const Frame&));

   private:
      using InputFunctionMap = QMap<Channel, InputFunction>;
      using OutputFunctionMap = QMap<Channel, OutputFunction>;

   private:
      virtual void internalCallback(const void* inputBuffer, void* outputBuffer, const Frame& framesPerBuffer);
      static int portAudioCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData);
      void startStream(const PaDeviceIndex& deviceId);
      PaDeviceIndex getDefaultDevice() const;
      void printDevices() const;

   private:
      static uint useCount;
      const PaDeviceInfo* device;
      PaStream* stream;
      float sampleRate;
      Frame framesPerBuffer;

      CallbackOverrideFunction callbackOverride;
      InputFunctionMap inputFunctionMap;
      AudioLoopFunction audioLoopFunction;
      OutputFunctionMap outputFunctionMap;
   };
} // namespace AudioDevice

#include "../Private/AudioDeviceDriver.hpp"

#endif // AudioDeviceDriverH
