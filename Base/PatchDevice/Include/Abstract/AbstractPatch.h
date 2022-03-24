#ifndef AbstractPatchH
#define AbstractPatchH

#ifndef NON_DAISY_DEVICE

#include <daisy_patch.h>

using AudioHandle = daisy::AudioHandle;
using DaisyPatch = daisy::DaisyPatch;
using DacHandle = daisy::DacHandle;

#else

#include <ImposterAudioHandle.h>
#include <ImposterDaisyPatch.h>

using AudioHandle = Imposter::AudioHandle;
using DaisyPatch = Imposter::DaisyPatch;

#endif // NON_DAISY_DEVICE

#include <Abstract/Abstract.h>
#include <Abstract/AbstractPageManager.h>

namespace Abstract
{
   class Patch
   {
   public:
      Patch(const std::string& appName);
      virtual ~Patch();

   public:
      void run();
      const std::string& getAppName() const;

   public:
      DaisyPatch daisy;

   protected:
      // called approx. every 1 ms
      virtual void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size);
      // called approx. every 5 ms
      virtual void nonAudioLoop();
      // called approx every 20 ms
      virtual void updateOled();

      Input compileInput();
      void applyOutput(const Output& output);
      Encoder compileEncoder(bool process = false);
      void updatePageManager(const Encoder& encoder);

      static Patch* the();

   protected:
      Page::Manager pageManager;

   private:
#ifdef NON_DAISY_DEVICE
      friend class Imposter::AudioHandle;
      friend class Imposter::DaisyPatch;
#endif // NON_DAISY_DEVICE

   private:
      void renderBootScreen(std::string appName);
      static void staticAudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size);

   private:
      static Patch* me;
      const std::string appName;
      bool longPressActive;
   };
} // namespace Abstract

#endif // AbstractPatchH
