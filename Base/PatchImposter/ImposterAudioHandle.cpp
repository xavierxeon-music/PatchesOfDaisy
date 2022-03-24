#include "ImposterAudioHandle.h"

#include <Abstract/AbstractPatch.h>

#include "ImposterDaisyPatch.h"

Imposter::AudioHandle::AudioHandle(DaisyPatch* imposterPatch)
   : inChannel()
   , outChannel()
   , imposterPatch(imposterPatch)
   , inBuffer(nullptr)
   , outBuffer(nullptr)
{
   for(uint8_t channel = 0; channel < 4; channel++)
      inChannel[channel].init(imposterPatch);
}

Imposter::AudioHandle::~AudioHandle()
{
   deleteBuffers();
}

void Imposter::AudioHandle::createBuffers()
{
   deleteBuffers();

   const size_t audioBlockSize = imposterPatch->AudioBlockSize();
   auto createBuffer = [&](float**& buffer)
   {
      buffer = new float*[4];
      for (uint8_t channel = 0; channel < 4; channel++)
      {
         buffer[channel] = new float[audioBlockSize];
         for (size_t block = 0; block < audioBlockSize; block++)
         {
            buffer[channel][block] = 0.0;
         }
      }
   };

   createBuffer(inBuffer);
   createBuffer(outBuffer);
}

void Imposter::AudioHandle::advanceInputBuffer()
{
   const size_t audioBlockSize = imposterPatch->AudioBlockSize();
   for (uint8_t channel = 0; channel < 4; channel++)
   {
      std::vector<float> values = inChannel[channel].process(audioBlockSize);
      for (size_t block = 0; block < audioBlockSize; block++)
      {
         inBuffer[channel][block] = values[block];
      }
   }

   Abstract::Patch::staticAudioCallback(inBuffer, outBuffer, audioBlockSize);

   for (uint8_t channel = 0; channel < 4; channel++)
   {
      outChannel[channel].process(outBuffer[channel], audioBlockSize);
   }
}

void Imposter::AudioHandle::deleteBuffers()
{
   auto deleteBuffer = [&](float**& buffer)
   {
      if (!buffer)
         return;
      for (uint8_t channel = 0; channel < 4; channel++)
      {
         delete[] buffer[channel];
      }
      delete[] buffer;
      buffer = nullptr;
   };

   deleteBuffer(inBuffer);
   deleteBuffer(outBuffer);
}
