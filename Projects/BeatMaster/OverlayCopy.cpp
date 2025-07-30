#include "OverlayCopy.h"

#include <Tools/Convert.h>
#include <Tools/Variable.h>

#include "Main.h"

OverlayCopy::OverlayCopy(Main* main)
   : Abstract::Page::Overlay()
   , main(main)
   , operation(CopyOperation::SeqIndexSource)
   , source{0, 0, Scope::Single}
   , target{0, 0, Scope::Single}
   , blink(10)
{
}

void OverlayCopy::render(OledDisplay* display)
{
   blink.valueAndNext();

   drawFrame(display, White);
   renderGrid(display, 10, source);
   renderGrid(display, 45, target);

   std::string copyText = "SINGLE";
   if (Scope::All == source.scope)
      copyText = "ALL";
   else if (Scope::Clear == source.scope)
      copyText = "CLEAR";

   if (Mode::Operation == mode && CopyOperation::ExecuteCopy == operation)
      write(display, 77, 20, "execute", Font_6x8, Black);
   else
      write(display, 77, 20, "-------", Font_6x8, White);

   write(display, 77, 35, copyText, Font_6x8, White);

   if (Mode::Operation == mode)
   {
      const bool color = blink.value();

      if (CopyOperation::SeqIndexSource == operation)
         display->DrawRect(10, 37, 34, 38, color, true);
      else if (CopyOperation::BankIndexSource == operation)
         display->DrawRect(10, 55, 34, 56, color, true);
      else if (CopyOperation::SeqIndexTarget == operation)
         display->DrawRect(45, 37, 69, 38, color, true);
      else if (CopyOperation::BankIndexTarget == operation)
         display->DrawRect(45, 55, 69, 56, color, true);
   }
}

void OverlayCopy::renderGrid(OledDisplay* display, const uint8_t xOffset, const Location& location)
{
   const uint8_t yOffset = 10;
   const uint8_t cellSize = 6;

   for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
   {
      const uint8_t col = midiChannel % 4;
      const uint8_t row = (midiChannel - col) / 4;

      const uint8_t x = xOffset + col * cellSize;
      const uint8_t y = yOffset + (3 * cellSize) - (row * cellSize);

      bool fill = false;
      if (Scope::Single == location.scope && location.midiChannel == midiChannel)
         fill = true;
      else if (Scope::All == location.scope)
         fill = true;

      display->DrawRect(x, y, x + cellSize, y + cellSize, White, fill);
   }

   const Main::BankIndex banks(location.bankIndex);

   const uint8_t y = yOffset + (6 * cellSize);
   for (uint8_t col = 0; col < 4; col++)
   {
      const uint8_t x = xOffset + col * cellSize;

      display->DrawRect(x, y, x + cellSize, y + cellSize, White, false);

      if (banks.get(col))
         display->DrawRect(x + 2, y + 2, x + 4, y + 4, White, true);
   }
}

Abstract::Page::State OverlayCopy::selected()
{
   source.midiChannel = target.midiChannel = main->getCurrentChannel();
   source.bankIndex = target.bankIndex = main->getBankIndex();
   source.scope = Scope::Single;

   return State::Turn;
}

Abstract::Page::State OverlayCopy::forward()
{
   if (CopyOperation::SeqIndexSource == operation)
   {
      if (Scope::Single == source.scope)
      {
         source.midiChannel++;
         if (source.midiChannel >= 16)
         {
            source.midiChannel = 0;
            source.scope = Scope::All;
         }
      }
      else if (Scope::All == source.scope)
         source.scope = Scope::Clear;
      else
         source.scope = Scope::Single;
   }
   else if (CopyOperation::BankIndexSource == operation)
   {
      Variable::Integer<uint8_t> var(source.bankIndex, 0, 15, true);
      var.increment();
   }
   else if (CopyOperation::SeqIndexTarget == operation)
   {
      target.midiChannel++;
      if (target.midiChannel >= 16)
         target.midiChannel = 0;
   }
   else if (CopyOperation::BankIndexTarget == operation)
   {
      Variable::Integer<uint8_t> var(target.bankIndex, 0, 15, true);
      var.increment();
   }

   return State::Overlay;
}

Abstract::Page::State OverlayCopy::back()
{
   if (CopyOperation::SeqIndexSource == operation)
   {
      if (Scope::Single == source.scope)
      {
         if (source.midiChannel == 0)
         {
            source.midiChannel = 15;
            source.scope = Scope::Clear;
         }
         else
            source.midiChannel--;
      }
      else if (Scope::Clear == source.scope)
         source.scope = Scope::All;
      else if (Scope::All == source.scope)
         source.scope = Scope::Single;
   }
   else if (CopyOperation::BankIndexSource == operation)
   {
      Variable::Integer<uint8_t> var(source.bankIndex, 0, 15, true);
      var.decrement();
   }
   else if (CopyOperation::SeqIndexTarget == operation)
   {
      if (0 == target.midiChannel)
         target.midiChannel = 15;
      else
         target.midiChannel--;
   }
   else if (CopyOperation::BankIndexTarget == operation)
   {
      Variable::Integer<uint8_t> var(target.bankIndex, 0, 15, true);
      var.decrement();
   }

   return State::Overlay;
}

Abstract::Page::State OverlayCopy::clicked()
{
   if (Mode::Neutral == mode)
   {
      mode = Mode::Operation;
      operation = CopyOperation::SeqIndexSource;
   }
   else
   {
      static const std::vector<CopyOperation> order = {CopyOperation::SeqIndexSource, CopyOperation::BankIndexSource, CopyOperation::SeqIndexTarget, CopyOperation::BankIndexTarget, CopyOperation::ExecuteCopy};

      Variable::Enum<CopyOperation> var(operation, order, true);
      var.increment();
   }

   return State::Overlay;
}

Abstract::Page::State OverlayCopy::longPressed()
{
   if (CopyOperation::ExecuteCopy == operation)
      executeCopy();

   operation = CopyOperation::SeqIndexSource;
   mode = Mode::Neutral;

   return State::Turn;
}

void OverlayCopy::executeCopy()
{
   if (Scope::Single == source.scope)
   {
      RhythmBank& sourceBank = main->getBank(source.bankIndex);
      Rhythm& sourceRhythm = sourceBank.getRhythm(source.midiChannel);

      RhythmBank& targetBank = main->getBank(target.bankIndex);
      Rhythm& targetRhythm = targetBank.getRhythm(target.midiChannel);

      targetRhythm.copyFromRhythm(sourceRhythm);
   }
   else if (Scope::All == source.scope)
   {
      RhythmBank& sourceBank = main->getBank(source.bankIndex);
      RhythmBank& targetBank = main->getBank(target.bankIndex);

      for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
      {
         Rhythm& sourceRhythm = sourceBank.getRhythm(midiChannel);
         Rhythm& targetRhythm = targetBank.getRhythm(midiChannel);

         targetRhythm.copyFromRhythm(sourceRhythm);
      }
   }
   else if (Scope::Clear == source.scope)
   {
      RhythmBank& sourceBank = main->getBank(source.bankIndex);

      for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
      {
         Rhythm& sourceRhythm = sourceBank.getRhythm(midiChannel);

         sourceRhythm.clear();
      }
   }
}
