#include "PageAbstract.h"

#include "Main.h"
#include "OverlayCopy.h"
#include "Rhythm.h"

PageAbstract::PageAbstract(Main* main)
   : Abstract::Page::Base()
   , main(main)
   , seqHeightMapper(0.0, 127.0, 0.0, 15.0)
   , blink(10)
   , saveIndicator(0, 30)
{
   setOverlay(new OverlayCopy(main));
}

void PageAbstract::renderSide(OledDisplay* display, const bool lowerScreen)
{
   blink.valueAndNext();

   const uint8_t xOffset = 0;
   const uint8_t yOffset = lowerScreen ? 36 : 0;

   const uint8_t& channel = main->getCurrentChannel();
   for (uint8_t midiChannel = 0; midiChannel < 16; midiChannel++)
   {
      const uint8_t col = midiChannel % 4;
      const uint8_t row = (midiChannel - col) / 4;

      const uint8_t x = xOffset + col * 4;
      const uint8_t y = yOffset + 12 - (row * 4);

      display->DrawRect(x, y, x + 4, y + 4, White, channel == midiChannel);
   }

   const Main::BankIndex& banks = main->getBankIndex();
   for (uint8_t col = 0; col < 4; col++)
   {
      const uint8_t x = xOffset + col * 4;
      const uint8_t y = yOffset + 20;

      display->DrawRect(x, y, x + 4, y + 4, White, false);

      if (banks.get(col))
         display->DrawRect(x + 1, y + 1, x + 3, y + 3, White, false);
   }

   saveIndicator.render(display, Remember::Root::isUnsynced());
}

Rhythm& PageAbstract::getCurrentRhythm() const
{
   const uint8_t bankIndex = main->getBankIndex();
   RhythmBank& bank = main->getBank(bankIndex);

   const uint8_t channel = main->getCurrentChannel();
   Rhythm& rhythm = bank.getRhythm(channel);

   return rhythm;
}
