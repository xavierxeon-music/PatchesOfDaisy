#include "PageOverview.h"

#include <Tools/Convert.h>

PageOverview::PageOverview(Harmonizer* main)
   : Abstract::Page::Base()
   , main(main)
   , saveIndicator(1, 35)
{
}

void PageOverview::render(OledDisplay* display)
{
   const Notes& notes = main->getNotes();
   for (uint8_t index = 0; index < 4; index++)
   {
      const uint8_t x = 30 * index;
      const uint8_t y = (0 == index % 2) ? 1 : 20;
      write(display, x, y, notes[index].name, Font_11x18, White);
   }

   saveIndicator.render(display, main->isUnsynced());

   std::string voices = " " + Convert::text(main->getNumberOfVoces()) + " voices";
   write(display, 0, 42, voices, Font_11x18, White);
}

Abstract::Page::State PageOverview::selected()
{
   return State::Page;
}

Abstract::Page::State PageOverview::forward()
{
   main->changeNumbertOfVoices(true);
   return State::Page;
}

Abstract::Page::State PageOverview::back()
{
   main->changeNumbertOfVoices(false);
   return State::Page;
}
