#include "ThePage.h"

#include <Tools/Convert.h>

#include "TestThingsHere.h"

ThePage::ThePage(TestThingsHere* main)
   : Abstract::Page::Base()
   , main(main)
{
}

void ThePage::render(OledDisplay* display)
{
   Abstract::Page::write(display, 0, 5, Convert::text(main->getVoltage(), 4), Font_7x10, Abstract::Page::White);
   Abstract::Page::write(display, 0, 20, Convert::text(main->getOffset(), 4), Font_7x10, Abstract::Page::White);
   Abstract::Page::write(display, 0, 35, Convert::text(main->getVoltage() + main->getOffset(), 4), Font_7x10, Abstract::Page::White);

   const std::string text = Convert::text(main->getIndex() + 1);
   Abstract::Page::write(display, 80, 15, text, Font_16x26, Abstract::Page::White);

   display->DrawRect(0, 50, 127, 63, White, true);
   Abstract::Page::write(display, 0, 53, main->getDeviceName(), Font_6x8, Abstract::Page::Black);
}

Abstract::Page::State ThePage::selected()
{
   return State::Page;
}

Abstract::Page::State ThePage::forward()
{
   main->changeIndex(true);
   return State::Page;
}

Abstract::Page::State ThePage::back()
{
   main->changeIndex(false);

   return State::Page;
}

Abstract::Page::State ThePage::clicked()
{
   main->changeDevice();
   return State::Page;
}
