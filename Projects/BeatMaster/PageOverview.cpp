#include "PageOverview.h"

#include "Main.h"

PageOverview::PageOverview(Main* main, const Tempo& tempo)
   : PageAbstract(main)
   , tempo(tempo)
{
}

void PageOverview::render(OledDisplay* display)
{
   const bool lowerScreen = (main->getCurrentChannel() >= 8);
   renderSide(display, lowerScreen);

   renderSequences(display);
}

void PageOverview::renderSequences(OledDisplay* display)
{
   const uint8_t bankIndex = main->getBankIndex();
   RhythmBank& bank = main->getBank(bankIndex);

   for (uint8_t channel = 0; channel < 16; channel++)
   {
      const uint8_t yDraw = 61 - (channel * 4);

      const Rhythm& rhythm = bank.getRhythm(channel);
      const uint8_t currentStep = rhythm.currentIndex();
      const uint8_t& numberOfSteps = rhythm.getNumberOfSteps();

      for (uint8_t step = 0; step < Rhythm::maxLength; step++)
      {
         if (step >= numberOfSteps)
            break;

         const uint8_t xDraw = 24 + (step * 4);
         const bool color = (step == currentStep && tempo.isRunningOrFirstTick()) ? blink.value() : White;

         const bool active = (rhythm.getVelocity(step) > 0);
         if (active)
            display->DrawRect(xDraw, yDraw, xDraw + 2, yDraw + 2, color, true);
         else
            display->DrawLine(xDraw, yDraw + 1, xDraw + 2, yDraw + 1, color);
      }

      if (main->getCurrentChannel() == channel)
      {
         const uint arrowY = yDraw + 1;
         const uint8_t arrowX = 12;
         const bool color = (Mode::Operation == mode) ? blink.value() : White;
         display->DrawLine(arrowX, arrowY, arrowX + 6, arrowY, color);
         display->DrawLine(arrowX + 5, arrowY - 1, arrowX + 5, arrowY + 1, color);
         display->DrawLine(arrowX + 4, arrowY - 2, arrowX + 4, arrowY + 2, color);
      }
   }
}

Abstract::Page::State PageOverview::forward()
{
   if (Mode::Operation == mode)
   {
      main->changeCurrentChannel(true);
   }
   return State::Page;
}

Abstract::Page::State PageOverview::back()
{
   if (Mode::Operation == mode)
   {
      main->changeCurrentChannel(false);
   }
   return State::Page;
}

Abstract::Page::State PageOverview::clicked()
{
   if (Mode::Neutral == mode)
   {
      mode = Mode::Operation;
      return State::Page;
   }
   else
   {
      mode = Mode::Neutral;
      return State::Turn;
   }
}
