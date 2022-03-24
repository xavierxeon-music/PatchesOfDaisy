#include "Private/AudioDeviceBuffers.h"

// common

BufferCommon::BufferCommon(const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel)
   : noOfFrames(noOfFrames)
   , channel(channel)
{
}

// input

const float& InputBufferImpl::operator[](const AudioDevice::Frame& frame) const
{
   const size_t index = channel + (frame * noOfInputChannels);
   return inputBuffer[index];
}

const AudioDevice::Frame& InputBufferImpl::getNoOfFrames() const
{
   return noOfFrames;
}

InputBufferImpl::InputBufferImpl(const float* inputBuffer, const int& noOfInputChannels, const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel)
   : AudioDevice::InputBuffer()
   , BufferCommon(noOfFrames, channel)
   , inputBuffer(inputBuffer)
   , noOfInputChannels(noOfInputChannels)
{
}

// output

float& OutputBufferImpl::operator[](const AudioDevice::Frame& frame)
{
   const size_t index = channel + (frame * noOfOutputChannels);
   return outputBuffer[index];
}

const AudioDevice::Frame& OutputBufferImpl::getNoOfFrames() const
{
   return noOfFrames;
}

OutputBufferImpl::OutputBufferImpl(float* outputBuffer, const int& noOfOutputChannels, const AudioDevice::Frame& noOfFrames, const AudioDevice::Channel& channel)
   : AudioDevice::OutputBuffer()
   , BufferCommon(noOfFrames, channel)
   , outputBuffer(outputBuffer)
   , noOfOutputChannels(noOfOutputChannels)
{
}
