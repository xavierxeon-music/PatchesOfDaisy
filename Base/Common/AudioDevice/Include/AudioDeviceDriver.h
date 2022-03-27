#ifndef AudioDeviceDriverH
#define AudioDeviceDriverH

#include <functional>

#include "../Private/AudioDeviceCommon.h"
#include "../portaudio/install/include/portaudio.h"

namespace AudioDevice
{
   // A sound interface
   // Both Gate and CV inputs, as well as outputs only work with a DC coupled interface!
   class Driver
   {      
   public:
      using AudioLoopFunction = std::function<void(const float& audioCallbackRate)>;
      using InputFunction = std::function<void(const InputBuffer& inputBuffer)>;
      using OutputFunction = std::function<void(OutputBuffer& inputBuffer)>;

      struct DeviceInfo
      {
         QString name;
         Common::SampleRate::BoolMap supportedSampleRates;
         float defaultSampleRate;

         DeviceInfo(const QString& name, const float& defaultSampleRate);

         using List = QList<DeviceInfo>;
      };

   public:
      Driver(const QString& deviceName, const float& sampleRate = Common::SampleRate::Default, const Frame& framesPerBuffer = 0);
      virtual ~Driver();

   public:
      static DeviceInfo::List listDevices();
      const float& getSampleRate() const;

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
      float sampleRate;
      Frame framesPerBuffer;

      InputFunctionMap inputFunctionMap;
      AudioLoopFunction audioLoopFunction;
      OutputFunctionMap outputFunctionMap;
   };
} // namespace AudioDevice

#include "../Private/AudioDeviceDriver.hpp"

#endif // AudioDeviceDriverH
