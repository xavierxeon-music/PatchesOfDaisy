#ifndef AudioDeviceBuffersH
#define AudioDeviceBuffersH

#include "../AudioDeviceDriver.h"

struct BufferCommon
{
   BufferCommon(const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel);

   const AudioDevice::Frame noOfFrames;
   const AudioDevice::Channel channel;
};

// input
struct InputBufferImpl : AudioDevice::InputBuffer, BufferCommon
{
   const float& operator[](const AudioDevice::Frame& frame) const override;
   const AudioDevice::Frame& getNoOfFrames() const override;

   InputBufferImpl(const float* inputBuffer, const int& noOfInputChannels, const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel);

   const float* inputBuffer;
   const int noOfInputChannels;
};

// output
struct OutputBufferImpl : AudioDevice::OutputBuffer, BufferCommon
{
   float& operator[](const AudioDevice::Frame& frame) override;
   const AudioDevice::Frame& getNoOfFrames() const override;

   OutputBufferImpl(float* outputBuffer, const int& noOfOutputChannels, const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel);

   float* outputBuffer;
   const int noOfOutputChannels;
};

#endif // AudioDeviceBuffersH
