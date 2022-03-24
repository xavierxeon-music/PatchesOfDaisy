#ifndef ImposterAudioHandleH
#define ImposterAudioHandleH

#include "AudioChannelInput.h"
#include "AudioChannelOutput.h"

namespace Imposter
{
   class DaisyPatch;

   struct SaiHandle
   {
      struct Config
      {
         enum class SampleRate
         {
            SAI_8KHZ,
            SAI_16KHZ,
            SAI_32KHZ,
            SAI_48KHZ,
            SAI_96KHZ
         };
      };
   };

   class AudioHandle
   {
   public:
      using InputBuffer = const float* const*;
      using OutputBuffer = float**;
      using AudioCallback = void (*)(InputBuffer in, OutputBuffer out, size_t size);

   public:
      AudioHandle(DaisyPatch* imposterPatch);
      ~AudioHandle();

   public:
      void createBuffers();
      void advanceInputBuffer();

   public:
      AudioChannel::Input inChannel[4];
      AudioChannel::Output outChannel[4];

   private:
      void deleteBuffers();

   private:
      DaisyPatch* imposterPatch;
      float** inBuffer;
      float** outBuffer;
   };
} // namespace Imposter

#endif // ImposterAudioHandleH
