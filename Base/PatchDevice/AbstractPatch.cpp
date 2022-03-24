#include <Abstract/AbstractPatch.h>

#include <Abstract/AbstractPage.h>
#include <Tools/Counter.h>

Abstract::Patch* Abstract::Patch::me = nullptr;

Abstract::Patch::Patch(const std::string& appName)
   : daisy()
   , pageManager()
   , appName(appName)
   , longPressActive(false)
{
   me = this;
   daisy.Init(); // Initialize hardware
   renderBootScreen(appName);
}

Abstract::Patch::~Patch()
{
   me = nullptr;
}

void Abstract::Patch::run()
{
#ifndef NON_DAISY_DEVICE
   daisy.StartAdc();
   daisy.StartAudio(&Abstract::Patch::staticAudioCallback);

   Counter oledCounter(4);

   while (true)
   {
      nonAudioLoop();
      if (oledCounter.nextAndIsMaxValue())
         updateOled();

      daisy.DelayMs(5);
   }
#else
   daisy.startUiMain();
#endif // NON_DAISY_DEVICE
}

const std::string& Abstract::Patch::getAppName() const
{
   return appName;
}

void Abstract::Patch::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   for (uint8_t channel = 0; channel < 4; channel++)
   {
      for (size_t chunk = 0; chunk < size; chunk++)
      {
         out[channel][chunk] = in[channel][chunk];
      }
   }
}

void Abstract::Patch::nonAudioLoop()
{
   // do nothing
}

void Abstract::Patch::updateOled()
{
   daisy.display.Fill(false);
   pageManager.render(&daisy.display);
   daisy.display.Update();
}

Input Abstract::Patch::compileInput()
{
   daisy.ProcessAllControls();

   Input input;

   // knobs
   for (int index = 0; index < 4; index++)
      input.controlVoltages[index] = 5.0 * daisy.controls[index].Value();

   // gates
   for (int index = 0; index < 2; index++)
      input.gates[index] = daisy.gate_input[index].Trig();

   return input;
}

void Abstract::Patch::applyOutput(const Output& output)
{
#ifndef NON_DAISY_DEVICE
   static const double scale = 4095.0;

   auto& dac = daisy.seed.dac;
   for (int index = 0; index < 2; index++)
      dac.WriteValue((DacHandle::Channel)index, static_cast<uint16_t>(0.2 * output.controlVoltages[index] * scale));

   daisy.seed.SetLed(output.led);
   dsy_gpio_write(&daisy.gate_output, output.gate);
#else
   daisy.applyOutput(output);
#endif // NON_DAISY_DEVICE
}

Encoder Abstract::Patch::compileEncoder(bool process)
{
   if (process)
      daisy.ProcessAllControls();

   const int32_t value = daisy.encoder.Increment();

   if (value > 0)
   {
      return Encoder::Forwad;
   }
   else if (value < 0)
   {
      return Encoder::Back;
   }
   else if (daisy.encoder.Pressed())
   {
      if (daisy.encoder.TimeHeldMs() > 500.0 && !longPressActive)
      {
         longPressActive = true;
         return Encoder::LongPressed;
      }
   }
   else if (daisy.encoder.FallingEdge())
   {
      daisy.encoder.Debounce();
      if (longPressActive)
      {
         longPressActive = false;
         return Encoder::None;
      }
      return Encoder::Clicked;
   }
   return Encoder::None;
}

void Abstract::Patch::updatePageManager(const Encoder& encoder)
{
   if (Encoder::Forwad == encoder)
      pageManager.forward();
   else if (Encoder::Back == encoder)
      pageManager.back();
   else if (Encoder::Clicked == encoder)
      pageManager.clicked();
   else if (Encoder::LongPressed == encoder)
      pageManager.longPressed();
}

Abstract::Patch* Abstract::Patch::the()
{
   return me;
}

void Abstract::Patch::renderBootScreen(std::string appName)
{
   static FontDef& splashFont = Font_11x18;

   daisy.display.Fill(Page::White);
   const uint8_t maxChars = static_cast<uint8_t>(static_cast<float>(Page::Base::width) / static_cast<float>(splashFont.FontWidth));
   appName = appName.substr(0, maxChars);

   const uint8_t x = Page::compileLeftX(63, appName, splashFont, Alignment::Center);
   Page::write(&(daisy.display), x, 30, appName, splashFont, Page::Black);

   daisy.display.Update();
}

void Abstract::Patch::staticAudioCallback(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   if (!me)
      return;

   me->audioLoop(in, out, size);
}
