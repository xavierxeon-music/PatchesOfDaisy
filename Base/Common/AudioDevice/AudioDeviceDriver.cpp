#include "AudioDeviceDriver.h"

#include "Private/AudioDeviceBuffers.h"

uint AudioDevice::Driver::useCount = 0;

AudioDevice::Driver::Driver(const QString& deviceName, float sampleRate, Frame framesPerBuffer)
   : device(nullptr)
   , stream(nullptr)
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
      qInfo() << "available devices are:";
      for (const DeviceInfo& info : listDevices())
      {
         auto stream = qInfo();
         stream << " * " << qPrintable(info.name) << ", sample rates [";
         if (info.cdSampleRate)
            stream << " CD ";
         if (info.normalSampleRate)
            stream << " Normal ";
         if (info.highSampleRate)
            stream << " High ";
         stream << "]";
      }

      return;
   }

   if (Common::SampleRateDefault == sampleRate)
      sampleRate = device->defaultSampleRate;
   if (0 == framesPerBuffer)
      framesPerBuffer = static_cast<Frame>(sampleRate / 1000.0);

   startStream(deviceId, sampleRate, framesPerBuffer);
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

AudioDevice::Driver::DeviceInfo::List AudioDevice::Driver::listDevices()
{
   DeviceInfo::List devices;
   if (0 == useCount) // local init
   {
      if (paNoError != Pa_Initialize())
         return devices;
   }

   const PaDeviceIndex counter = Pa_GetDeviceCount();
   for (PaDeviceIndex index = 0; index < counter; index++)
   {
      const PaDeviceInfo* paInfo = Pa_GetDeviceInfo(index);
      DeviceInfo info = {paInfo->name, false, false, false};

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

      auto testSampleRate = [&](float sampleRate, bool& marker)
      {
         PaError result = Pa_IsFormatSupported(inputParameterPointer, outputParameterPointer, sampleRate);
         marker = (paFormatIsSupported == result);
      };

      testSampleRate(Common::SampleRateCD, info.cdSampleRate);
      testSampleRate(Common::SampleRateNormal, info.normalSampleRate);
      testSampleRate(Common::SampleRateHigh, info.highSampleRate);

      devices.append(info);
   }

   if (0 == useCount) // local deinit
      Pa_Terminate();

   return devices;
}

float AudioDevice::Driver::getSampleRate() const
{
   if (!device)
      return 0.0;

   return device->defaultSampleRate;
}

void AudioDevice::Driver::registerInputFunction(InputFunction inputFunction, const Channel& channel)
{
   inputFunctionMap[channel] = inputFunction;
}

void AudioDevice::Driver::registerOutputFunction(OutputFunction outputFunction, const Channel& channel)
{
   outputFunctionMap[channel] = outputFunction;
}

void AudioDevice::Driver::internalCallback(const void* inputBuffer, void* outputBuffer, Frame framesPerBuffer)
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
      const float audioCallackRate = device->defaultSampleRate / framesPerBuffer;
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
      driver->internalCallback(inputBuffer, outputBuffer, framesPerBuffer);

   return paContinue;
}

void AudioDevice::Driver::startStream(const PaDeviceIndex& deviceId, const float& sampleRate, const Frame& framesPerBuffer)
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

   PaError result = Pa_OpenStream(&stream, &inputParameters, &outputParameters, sampleRate, framesPerBuffer, paNoFlag, &Driver::portAudioCallback, this);
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
