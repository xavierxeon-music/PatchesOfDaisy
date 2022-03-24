#ifndef ImposterDaisyPatchH
#define ImposterDaisyPatchH

#include <Abstract/Abstract.h>

#include "ImposterAnalogControl.h"
#include "ImposterAudioHandle.h"
#include "ImposterDaisySeed.h"
#include "ImposterEncoder.h"
#include "ImposterGateIn.h"
#include "ImposterOledDisplay.h"

namespace Abstract
{
   class Patch;
}

class MainWindow;

namespace Imposter
{
   class DaisyPatch
   {
   public:
      enum Ctrl
      {
         CTRL_1,
         CTRL_2,
         CTRL_3,
         CTRL_4,
         CTRL_LAST
      };

      enum GateInput
      {
         GATE_IN_1,
         GATE_IN_2,
         GATE_IN_LAST
      };

   public:
      DaisyPatch();

   public: // daisy facade
      void Init();
      void DelayMs(size_t del);
      void SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate);
      float AudioSampleRate();
      void SetAudioBlockSize(size_t size);
      size_t AudioBlockSize();
      float AudioCallbackRate();
      void ProcessAllControls();

   public: // other
      void startUiMain();
      void executeAudioStep();
      void executeNonAudioStep(bool updateDisplay);
      void applyOutput(const Output& output);
      static QString getAppName();

   public:
      DaisySeed seed;
      Encoder encoder;
      AnalogControl controls[CTRL_LAST];
      GateIn gate_input[GATE_IN_LAST];
      // use midi classes
      OledDisplay display;
      // gate out missing

      float sampleRate; // dampleRate must be set before audio handle
      size_t audioBlockSize;
      AudioHandle audioHandle;

   private:
      MainWindow* mainWindow;
   };
} // namespace Imposter

#endif // ImposterDaisyPatchH
