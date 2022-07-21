#include "AudioDeviceDriver.h"

#include "Private/AudioDeviceBuffers.h"

// device info

AudioDevice::Driver::DeviceInfo::DeviceInfo(const QString& name, const float& defaultSampleRate)
   : name(name)
   , supportedSampleRates()
   , defaultSampleRate(defaultSampleRate)
{
}

// driver

uint AudioDevice::Driver::useCount = 0;

AudioDevice::Driver::Driver(const QString& deviceName, const float& sampleRate, const Frame& framesPerBuffer)
   : device(nullptr)
   , stream(nullptr)
   , sampleRate(sampleRate)
   , framesPerBuffer(framesPerBuffer)
   , callbackOverride(nullptr)
   , inputFunctionMap()
   , audioLoopFunction(nullptr)
   , outputFunctionMap()
{   
   if (0 == useCount)
   {
      if (paNoError != Pa_Initialize())
      {
         qFatal("Error while initialiazing portaudio");
         return;
      }
   }
   useCount++;

   PaDeviceIndex deviceId = -1;
   const PaDeviceIndex counter = Pa_GetDeviceCount();
   for (PaDeviceIndex index = 0; index < counter; index++)
   {
      const PaDeviceInfo* testInfo = Pa_GetDeviceInfo(index);
      const QString infoName(testInfo->name);
      if (infoName != deviceName)
         continue;

      deviceId = index;
      device = testInfo;
      break;
   }

   if (-1 == deviceId)
   {
      qWarning() << "unable to find device" << deviceName;
      qInfo() << "available devices (with default rate in CAPITAL) are:";
      const PaDeviceIndex defaultDeviceId = Pa_GetDefaultOutputDevice();
      const DeviceInfo::Map devices = listDevices();
      for (DeviceInfo::Map::const_iterator it = devices.constBegin(); it != devices.constEnd(); it++)
      {
         const DeviceInfo& info = it.value();

         auto stream = qInfo();
         if (defaultDeviceId == it.key() && deviceName.isEmpty())
         {
            stream << " @ ";
            deviceId = defaultDeviceId;
         }
         else
            stream << " * ";
         stream << qPrintable(info.name) << ", sample rates [";

         for (Common::SampleRate::BoolMap::const_iterator it = info.supportedSampleRates.constBegin(); it != info.supportedSampleRates.constEnd(); it++)
         {
            const bool supported = it.value();
            if (!supported)
               continue;

            const bool isDefault = (it.key() == info.defaultSampleRate);
            const QString name = Common::SampleRate::nameMap[it.key()];
            if (isDefault)
               stream << qPrintable(name.toUpper());
            else
               stream << qPrintable(name.toLower());
         }
         stream << "]";
      }
   }

   if (-1 == deviceId) // device not found
      return;

   if (Common::SampleRate::Default == sampleRate)
      this->sampleRate = device->defaultSampleRate;
   if (0 == framesPerBuffer)
      this->framesPerBuffer = static_cast<Frame>(sampleRate / 1000.0);

   startStream(deviceId);
}

AudioDevice::Driver::~Driver()
{
   if (stream)
   {
      Pa_StopStream(stream);
      Pa_CloseStream(stream);

      qInfo() << "closed audio device" << device->name;
   }

   useCount--;
   if (0 == useCount)
      Pa_Terminate();
}

AudioDevice::Driver::DeviceInfo::Map AudioDevice::Driver::listDevices()
{
   DeviceInfo::Map devices;
   if (0 == useCount) // local init
   {
      if (paNoError != Pa_Initialize())
         return devices;
   }

   const PaDeviceIndex counter = Pa_GetDeviceCount();
   for (PaDeviceIndex index = 0; index < counter; index++)
   {
      const PaDeviceInfo* paInfo = Pa_GetDeviceInfo(index);
      DeviceInfo info(paInfo->name, paInfo->defaultSampleRate);

      PaStreamParameters inputParameters;
      inputParameters.device = index;
      inputParameters.channelCount = paInfo->maxInputChannels;
      inputParameters.sampleFormat = paFloat32;
      inputParameters.suggestedLatency = paInfo->defaultLowInputLatency;
      inputParameters.hostApiSpecificStreamInfo = nullptr;

      PaStreamParameters outputParameters;
      outputParameters.device = index;
      outputParameters.channelCount = paInfo->maxOutputChannels;
      outputParameters.sampleFormat = paFloat32;
      outputParameters.suggestedLatency = paInfo->defaultLowOutputLatency;
      outputParameters.hostApiSpecificStreamInfo = nullptr;

      PaStreamParameters* inputParameterPointer = (0 == paInfo->maxInputChannels) ? nullptr : &inputParameters;
      PaStreamParameters* outputParameterPointer = (0 == paInfo->maxOutputChannels) ? nullptr : &outputParameters;

      auto testSampleRate = [&](const Common::SampleRate::Value& testSampleRate)
      {
         PaError result = Pa_IsFormatSupported(inputParameterPointer, outputParameterPointer, testSampleRate);
         const bool supported = (paFormatIsSupported == result);

         info.supportedSampleRates[testSampleRate] = supported;
      };

      testSampleRate(Common::SampleRate::CompactDisk);
      testSampleRate(Common::SampleRate::Normal);
      testSampleRate(Common::SampleRate::High);

      devices.insert(index, info);
   }

   if (0 == useCount) // local deinit
      Pa_Terminate();

   return devices;
}

const float& AudioDevice::Driver::getSampleRate() const
{
   return sampleRate;
}

const int& AudioDevice::Driver::getMaxInputChannels() const
{
   return device->maxInputChannels;
}

const int& AudioDevice::Driver::getMaxOutputChannels() const
{
   return device->maxOutputChannels;
}

void AudioDevice::Driver::registerInputFunction(InputFunction inputFunction, const Channel& channel)
{
   inputFunctionMap[channel] = inputFunction;
}

void AudioDevice::Driver::registerOutputFunction(OutputFunction outputFunction, const Channel& channel)
{
   outputFunctionMap[channel] = outputFunction;
}

void AudioDevice::Driver::internalCallback(const void* inputBuffer, void* outputBuffer, const Frame& framesPerBuffer)
{
   for (int channel = 0; channel < device->maxInputChannels; channel++)
   {
      if (!inputFunctionMap.contains(channel))
         continue;

      InputBufferImpl buffer(static_cast<const float*>(inputBuffer), device->maxInputChannels, framesPerBuffer, channel);
      inputFunctionMap[channel](buffer);
   }

   if (audioLoopFunction)
   {
      const float audioCallackRate = sampleRate / framesPerBuffer;
      audioLoopFunction(audioCallackRate);
   }

   for (int channel = 0; channel < device->maxOutputChannels; channel++)
   {
      if (!outputFunctionMap.contains(channel))
         continue;

      OutputBufferImpl buffer(static_cast<float*>(outputBuffer), device->maxOutputChannels, framesPerBuffer, channel);
      outputFunctionMap[channel](buffer);
   }
}

int AudioDevice::Driver::portAudioCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void* userData)
{
   Q_UNUSED(timeInfo)
   Q_UNUSED(statusFlags)

   Driver* driver = reinterpret_cast<Driver*>(userData);
   if (driver)
   {
      if (driver->callbackOverride)
         driver->callbackOverride(inputBuffer, outputBuffer, framesPerBuffer);
      else
         driver->internalCallback(inputBuffer, outputBuffer, framesPerBuffer);
   }

   return paContinue;
}

void AudioDevice::Driver::startStream(const PaDeviceIndex& deviceId)
{
   PaStreamParameters inputParameters;
   {
      inputParameters.device = deviceId;
      inputParameters.sampleFormat = paFloat32;
      inputParameters.hostApiSpecificStreamInfo = nullptr;

      inputParameters.channelCount = device->maxInputChannels;
      inputParameters.suggestedLatency = device->defaultLowInputLatency;
   }

   PaStreamParameters outputParameters;
   {
      outputParameters.device = deviceId;
      outputParameters.sampleFormat = paFloat32;
      outputParameters.hostApiSpecificStreamInfo = nullptr;

      outputParameters.channelCount = device->maxOutputChannels;
      outputParameters.suggestedLatency = device->defaultLowOutputLatency;
   }

   PaStreamParameters* inputParameterPointer = (0 == device->maxInputChannels) ? nullptr : &inputParameters;
   PaStreamParameters* outputParameterPointer = (0 == device->maxOutputChannels) ? nullptr : &outputParameters;

   PaError result = Pa_OpenStream(&stream, inputParameterPointer, outputParameterPointer, sampleRate, framesPerBuffer, paNoFlag, &Driver::portAudioCallback, this);
   if (paNoError != result)
   {
      qWarning() << "unable to open stream" << Pa_GetErrorText(result);
      return;
   }

   result = Pa_StartStream(stream);
   if (paNoError != result)
   {
      stream = nullptr;

      qWarning() << "unable to start stream" << Pa_GetErrorText(result);
      return;
   }

   qInfo() << "opened audio device" << device->name;
}
