#include "PageSequence.h"

#include <Music/Tempo.h>
#include <Tools/Convert.h>
#include <Tools/Variable.h>

#include "Channel.h"
#include "Key.h"
#include "RelativeEight.h"
#include "Sequence.h"

PageSequence::PageSequence(RelativeEight* main)
   : Abstract::Page::Base()
   , main(main)
   , operation(SeqeunceOperation::NumberOfSteps)
   , blink(10)
   , saveIndicator(125, 0)
   , editStepIndex(0)
{
}

void PageSequence::render(OledDisplay* display)
{
   blink.valueAndNext();

   Sequence* sequence = main->getCurrentSequence();

   write(display, 0, 0, "count", Font_6x8, White);
   const uint8_t count = sequence->getNumberOfSteps();
   bool stepCountColor = White;
   if (SeqeunceOperation::NumberOfSteps == operation)
   {
      if (Mode::Select == mode)
         stepCountColor = Black;
      else if (Mode::Operation == mode)
         stepCountColor = blink.value();
   }
   write(display, 35, 0, Convert::text(count), Font_6x8, stepCountColor);

   write(display, 54, 0, "size", Font_6x8, White);
   const Tempo::Division stepSize = sequence->getStepSize();
   bool stepSizeColor = White;
   if (SeqeunceOperation::StepSize == operation)
   {
      if (Mode::Select == mode)
         stepSizeColor = Black;
      else if (Mode::Operation == mode)
         stepSizeColor = blink.value();
   }
   write(display, 85, 0, Tempo::getName(stepSize), Font_6x8, stepSizeColor);

   const uint8_t selectedChannel = main->getChannel(1)->selection;
   write(display, 115, 0, Channel::names.at(selectedChannel), Font_6x8, White);

   renderKeyboard(display);
   renderSequence(display);

   saveIndicator.render(display, Remember::Root::isUnsynced());
}

void PageSequence::renderKeyboard(OledDisplay* display)
{
   const uint8_t xStart = 0;
   const uint8_t yStart = 9;

   // top
   display->DrawRect(xStart, yStart + 0, xStart + (24 * 5), yStart + 3, White, true);
   // bottom
   display->DrawRect(xStart, yStart + 51, xStart + (24 * 5), yStart + 54, White, true);

   for (uint8_t index = 0; index < 24; index++)
   {
      const uint8_t x = xStart + (5 * index);
      const uint8_t keyIndex = index % 12;
      const bool black = Key::map.at(keyIndex).black;
      if (!black)
      {
         // top
         display->DrawRect(x, yStart + 1, x + 5, yStart + 5, Black, false);
         display->DrawRect(x + 1, yStart + 2, x + 4, yStart + 5, White, true);

         // bottom
         display->DrawRect(x, yStart + 48, x + 5, yStart + 53, Black, false);
         display->DrawRect(x + 1, yStart + 49, x + 4, yStart + 52, White, true);
      }
   }
}

void PageSequence::renderSequence(OledDisplay* display)
{
   const uint8_t xStart = 1;
   const uint8_t yStart = 17;

   Sequence* sequence = main->getCurrentSequence();

   const uint8_t maxIndex = sequence->getNumberOfSteps();
   for (uint8_t index = 0; index < maxIndex; index++)
   {
      const uint8_t x = xStart + (5 * sequence->getStepValue(index));
      const uint8_t y = yStart + (5 * index);

      if (SeqeunceOperation::NoteSelect != operation)
         display->DrawRect(x, y, x + 3, y + 3, White, false);
      else
      {
         bool color = White;
         if (Mode::Operation == mode && index == editStepIndex)
            color = blink.value();

         display->DrawRect(x, y, x + 3, y + 3, color, false);

         if (Mode::Select == mode && index == editStepIndex)
            display->DrawRect(120, y, 128, y + 3, White, blink.value());
      }
   }
}

Abstract::Page::State PageSequence::forward()
{
   Sequence* sequence = main->getCurrentSequence();

   if (Mode::Select == mode)
   {
      if (SeqeunceOperation::NumberOfSteps == operation)
         operation = SeqeunceOperation::StepSize;
      else if (SeqeunceOperation::StepSize == operation)
         operation = SeqeunceOperation::NoteSelect;
      else if (SeqeunceOperation::NoteSelect == operation)
      {
         Variable::Integer<uint8_t> var(editStepIndex, 0, sequence->getNumberOfSteps() - 1, false);
         if (!var.change(true))
            mode = Mode::Neutral;
      }
   }
   else if (Mode::Operation == mode)
   {
      if (SeqeunceOperation::NumberOfSteps == operation)
         sequence->changeNumberOfSteps(true);
      else if (SeqeunceOperation::StepSize == operation)
         sequence->changeStepSize(true);
      else if (SeqeunceOperation::NoteSelect == operation)
         sequence->changeStepValue(editStepIndex, true);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageSequence::back()
{
   Sequence* sequence = main->getCurrentSequence();

   if (Mode::Select == mode)
   {
      if (SeqeunceOperation::NumberOfSteps == operation)
         mode = Mode::Neutral;
      else if (SeqeunceOperation::StepSize == operation)
         operation = SeqeunceOperation::NumberOfSteps;
      else if (SeqeunceOperation::NoteSelect == operation)
      {
         Variable::Integer<uint8_t> var(editStepIndex, 0, sequence->getNumberOfSteps() - 1, false);
         if (!var.change(false))
            operation = SeqeunceOperation::StepSize;
      }
   }
   else if (Mode::Operation == mode)
   {
      if (SeqeunceOperation::NumberOfSteps == operation)
         sequence->changeNumberOfSteps(false);
      else if (SeqeunceOperation::StepSize == operation)
         sequence->changeStepSize(false);
      else if (SeqeunceOperation::NoteSelect == operation)
         sequence->changeStepValue(editStepIndex, false);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageSequence::clicked()
{
   if (Mode::Neutral == mode)
   {
      mode = Mode::Select;
      operation = SeqeunceOperation::NumberOfSteps;
      editStepIndex = 0;
   }
   else if (Mode::Select == mode)
      mode = Mode::Operation;
   else if (Mode::Operation == mode)
      mode = Mode::Select;

   return turnIfNeutral();
}
