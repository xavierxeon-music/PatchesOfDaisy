#include "PageGates.h"

#include <Tools/Convert.h>
#include <Tools/Variable.h>

#include "Icons.h"
#include "Main.h"

PageGates::PageGates(Main* main, const Tempo& tempo)
   : Abstract::Page::Base()
   , main(main)
   , tempo(tempo)
   , saveIndicator(0, 60)
   , operation(GateOperation::Channel)
   , audioChannel(0)
   , blink(10)
   , percentMapper(0, 1.0, 6.0, 30.0)
{
}

void PageGates::render(OledDisplay* display)
{
   blink.valueAndNext();

   for (uint8_t index = 0; index < 4; index++)
      renderGate(display, index);

   static const uint8_t yText = 50;

   if (Mode::Operation == mode)
   {
      Gate& gate = main->getGate(audioChannel);

      const uint8_t wait = gate.getStageLength(Gate::Stage::Wait);
      write(display, 0, yText, Convert::text(wait), Font_7x10, (GateOperation::Wait == operation) ? Black : White);

      write(display, 35, yText, "/", Font_7x10, White);
      const uint8_t top = gate.getStageLength(Gate::Stage::High);
      write(display, 45, yText, Convert::text(top), Font_7x10, (GateOperation::Top == operation) ? Black : White);

      write(display, 80, yText, "/", Font_7x10, White);
      const uint8_t zero = gate.getStageLength(Gate::Stage::Low);
      write(display, 90, yText, Convert::text(zero), Font_7x10, (GateOperation::Zero == operation) ? Black : White);
   }
   else
   {
      const uint8_t bar = tempo.getCounter(Tempo::Division::Bar) % 1000;
      const uint8_t quarter = tempo.getCounter(Tempo::Division::Quarter);

      const std::string counterText = Convert::text(bar + 1) + ":" + Convert::text(quarter + 1);
      const uint8_t counterX = compileLeftX(60, counterText, Font_7x10, Alignment::Right);
      write(display, counterX, yText, counterText, Font_7x10, White);

      const uint8_t bpm = tempo.getBeatsPerMinute();
      const std::string bpmText = Convert::text(bpm);

      const uint8_t bpmX = compileLeftX(95, bpmText, Font_7x10, Alignment::Right);
      write(display, bpmX, yText, bpmText, Font_7x10, White);
      write(display, 95, yText, " bpm", Font_7x10, White);
   }

   saveIndicator.render(display, Remember::Root::isUnsynced());
}

void PageGates::renderGate(OledDisplay* display, const uint8_t index)
{
   static const uint8_t width = 32;
   const uint8_t xStart = index * width;

   Gate& gate = main->getGate(index);
   const Gate::Stage& stage = gate.getCurrentStage();

   // channel
   if (Mode::Operation == mode && index == audioChannel)
   {
      const bool channelColor = (GateOperation::Channel == operation && Mode::Operation == mode) ? blink.value() : White;
      display->DrawRect(xStart, 0, xStart + width, 1, channelColor, true);
   }

   // knobs
   bool knobColor = White;
   if (GateOperation::Knobs == operation && Mode::Operation == mode && index == audioChannel)
      knobColor = Black;

   if (gate.getApplyToKnobs())
      Icons::render(display, Icons::ChannelActive, xStart, 5, knobColor);
   else
      Icons::render(display, Icons::ChannelInactive, xStart, 5, knobColor);

   // stage
   if (Gate::Stage::High == stage && gate.isValid())
      Icons::render(display, Icons::ArrowUp, xStart, 20, White);
   else if (Gate::Stage::Low == stage && gate.isValid())
      Icons::render(display, Icons::ArrowDown, xStart, 20, White);
   else
      Icons::render(display, Icons::Pause, xStart, 20, White);

   // percentage
   if (gate.isValid())
   {
      const float percentage = gate.getCurrentStagePercentage();
      if (Gate::Stage::High == gate.getCurrentStage())
      {
         const uint8_t percentY = percentMapper(1.0 - percentage);
         display->DrawRect(xStart + 14, percentY, xStart + 16, 30, White, true);
      }
      else
      {
         const uint8_t percentY = percentMapper(percentage);
         display->DrawRect(xStart + 14, percentY, xStart + 16, 30, White, true);
      }
   }
}

Abstract::Page::State PageGates::forward()
{
   Gate& gate = main->getGate(audioChannel);

   if (GateOperation::Channel == operation)
      changeChannel(true);
   else if (GateOperation::Knobs == operation)
      gate.toggleApplyToKnobs();
   else if (GateOperation::Wait == operation)
      gate.changeStageLength(Gate::Stage::Wait, true);
   else if (GateOperation::Top == operation)
      gate.changeStageLength(Gate::Stage::High, true);
   else if (GateOperation::Zero == operation)
      gate.changeStageLength(Gate::Stage::Low, true);

   return State::Page;
}

Abstract::Page::State PageGates::back()
{
   Gate& gate = main->getGate(audioChannel);

   if (GateOperation::Channel == operation)
      changeChannel(false);
   else if (GateOperation::Knobs == operation)
      gate.toggleApplyToKnobs();
   else if (GateOperation::Wait == operation)
      gate.changeStageLength(Gate::Stage::Wait, false);
   else if (GateOperation::Top == operation)
      gate.changeStageLength(Gate::Stage::High, false);
   else if (GateOperation::Zero == operation)
      gate.changeStageLength(Gate::Stage::Low, false);

   return State::Page;
}

Abstract::Page::State PageGates::clicked()
{
   if (Mode::Neutral == mode)
   {
      mode = Mode::Operation;
      operation = GateOperation::Channel;
   }
   else
   {
      static const std::vector<GateOperation> orderWithKnobs = {GateOperation::Channel, GateOperation::Knobs, GateOperation::Wait, GateOperation::Top, GateOperation::Zero};

      Variable::Enum<GateOperation> var(operation, orderWithKnobs, false);
      if (!var.increment())
         mode = Mode::Neutral;
   }
   return turnIfNeutral();
}

void PageGates::changeChannel(bool up)
{
   Variable::Integer<uint8_t> var(audioChannel, 0, 4, true);
   var.change(up);
}
