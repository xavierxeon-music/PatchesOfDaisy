#include "AudioDeviceDriver.h"

#include "Private/AudioDeviceBuffers.h"

uint AudioDevice::Driver::useCount = 0;
static const unsigned long framesPerBuffer = 2;

AudioDevice::Driver::Driver(const QString& deviceName)
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
      break;
   }

   if (-1 == deviceId)
   {
      qWarning() << "unable to find device" << deviceName;
      qInfo() << "available devices are:";
      for (const QString& name : listDevices())
         qInfo() << " * " << name;

      return;
   }

   device = Pa_GetDeviceInfo(deviceId);
   startStream(deviceId);
}

AudioDevice::Driver::~Driver()
{
   if (stream)
   {
      Pa_StopStream(stream);
      Pa_CloseStream(stream);
   }

   useCount--;
   if (0 == useCount)
      Pa_Terminate();
}

QStringList AudioDevice::Driver::listDevices()
{
   QStringList devices;
   if (0 == useCount) // local init
   {
      if (paNoError != Pa_Initialize())
         return devices;
   }

   const PaDeviceIndex counter = Pa_GetDeviceCount();
   for (PaDeviceIndex index = 0; index < counter; index++)
   {
      const PaDeviceInfo* info = Pa_GetDeviceInfo(index);
      devices.append(info->name);
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

   PaError result = Pa_OpenStream(&stream, &inputParameters, &outputParameters, device->defaultSampleRate, framesPerBuffer, paNoFlag, &Driver::portAudioCallback, this);
   if (paNoError != result)
   {
      qWarning() << "unable to open stream";
      return;
   }

   result = Pa_StartStream(stream);
   if (paNoError != result)
   {
      stream = nullptr;

      qWarning() << "unable to start stream";
      return;
   }
}
