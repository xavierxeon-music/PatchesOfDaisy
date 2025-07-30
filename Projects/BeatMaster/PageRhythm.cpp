#include "PageRhythm.h"

#include <Tools/Convert.h>
#include <Tools/Variable.h>

#include "Main.h"

PageRhythm::PageRhythm(Main* main)
   : PageAbstract(main)
   , operation(RhythmOperation::NumberOfSteps)
   , currentStep(0)
{
}

void PageRhythm::render(OledDisplay* display)
{
   renderSide(display);

   renderSettings(display);
   renderSequence(display);
}

void PageRhythm::renderSettings(OledDisplay* display)
{
   const uint8_t yStart = 5;
   const uint8_t rowHeight = 12;

   Rhythm& rhythm = getCurrentRhythm();

   // steps count
   const uint8_t yStepCount = yStart + (0 * rowHeight);
   const std::string stepCountText = "steps";
   const uint8_t xStepCount = compileLeftX(65, stepCountText, Font_7x10, Alignment::Right);
   write(display, xStepCount, yStepCount, stepCountText, Font_7x10, White);

   write(display, 70, yStepCount, Convert::text(rhythm.getNumberOfSteps()), Font_7x10, (Mode::Operation == mode && RhythmOperation::NumberOfSteps == operation) ? Black : White);

   // steps size
   const uint8_t yStepSize = yStart + (1 * rowHeight);
   const std::string stepSizeText = "size";
   const uint8_t xStepSize = compileLeftX(65, stepSizeText, Font_7x10, Alignment::Right);
   write(display, xStepSize, yStepSize, stepSizeText, Font_7x10, White);

   write(display, 70, yStepSize, Tempo::getName(rhythm.getStepSize()), Font_7x10, (Mode::Operation == mode && RhythmOperation::StepSize == operation) ? Black : White);
}

void PageRhythm::renderSequence(OledDisplay* display)
{
   Rhythm& rhythm = getCurrentRhythm();

   const uint8_t y = 38;
   const uint8_t& numberOfSteps = rhythm.getNumberOfSteps();

   for (uint8_t step = 0; step < Rhythm::maxLength; step++)
   {
      if (step >= numberOfSteps)
         break;

      const float& velocity = rhythm.getVelocity(step);

      const uint8_t x = step * 5;
      const uint8_t height = seqHeightMapper(velocity);

      bool color = White;
      if (RhythmOperation::Velocity == operation && step == currentStep)
      {
         if (Mode::Select == mode)
            color = blink.value();
         else
            display->DrawRect(x + 0, y + 18, x + 3, y + 22, blink.value(), true);
      }
      display->DrawRect(x + 0, y + (16 - height), x + 3, y + 16, color, true);

      if (0 == step % 4) // step helper markers
         display->DrawRect(x + 1, y + 20, x + 2, y + 22, White, true);
   }

   const uint8_t x = numberOfSteps * 5;
   display->DrawLine(x, y + 10, 127, y + 10, White);
}

Abstract::Page::State PageRhythm::forward()
{
   Rhythm& rhythm = getCurrentRhythm();

   if (Mode::Operation == mode)
   {
      if (RhythmOperation::NumberOfSteps == operation)
         rhythm.changeNumberOfSteps(true);
      else if (RhythmOperation::StepSize == operation)
         rhythm.changeStepSize(true);
      else if (RhythmOperation::Velocity == operation)
      {
         Variable::Integer<uint8_t> var(currentStep, 0, rhythm.getNumberOfSteps() - 1, false);
         if (!var.change(true))
         {
            mode = Mode::Neutral;
            operation = RhythmOperation::NumberOfSteps;
            currentStep = 0;
         }
      }
   }
   else if (Mode::Select == mode)
   {
      rhythm.changeVelocity(currentStep, true);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageRhythm::back()
{
   Rhythm& rhythm = getCurrentRhythm();

   if (Mode::Operation == mode)
   {
      if (RhythmOperation::NumberOfSteps == operation)
         rhythm.changeNumberOfSteps(false);
      else if (RhythmOperation::StepSize == operation)
         rhythm.changeStepSize(false);
      else if (RhythmOperation::Velocity == operation)
      {
         Variable::Integer<uint8_t> var(currentStep, 0, rhythm.getNumberOfSteps() - 1, false);
         if (!var.change(false))
         {
            mode = Mode::Neutral;
            operation = RhythmOperation::NumberOfSteps;
            currentStep = 0;
         }
      }
   }
   else if (Mode::Select == mode)
   {
      rhythm.changeVelocity(currentStep, false);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageRhythm::clicked()
{
   if (Mode::Neutral == mode)
   {
      mode = Mode::Operation;
      operation = RhythmOperation::NumberOfSteps;
   }
   else if (RhythmOperation::Velocity != operation)
   {
      static const std::vector<RhythmOperation> order = {RhythmOperation::NumberOfSteps, RhythmOperation::StepSize, RhythmOperation::Velocity};

      Variable::Enum<RhythmOperation> var(operation, order, false);
      var.increment();
   }
   else
   {
      if (Mode::Operation == mode)
         mode = Mode::Select;
      else
         mode = Mode::Operation;
   }

   return turnIfNeutral();
}
