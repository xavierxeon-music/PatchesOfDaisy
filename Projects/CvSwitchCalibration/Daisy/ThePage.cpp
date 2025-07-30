#include "ThePage.h"

#include <Tools/Convert.h>

#include "CvSwitchCalibration.h"

ThePage::ThePage(CvSwitchCalibration* main)
   : Abstract::Page::Base()
   , main(main)
{
}

void ThePage::render(OledDisplay* display)
{
   const bool manualOffset = main->getManualOffset();

   display->DrawRect(0, 0, 31, 1, White, true);
   display->DrawRect(96, 0, 127, 1, White, true);

   Abstract::Page::write(display, 10, 15, Convert::text(main->getVoltage(), 4), Font_7x10, Abstract::Page::White);
   if (manualOffset)
      Abstract::Page::write(display, 10, 30, Convert::text(main->getOffset(), 4), Font_7x10, Abstract::Page::White);

   const std::string text = Convert::text(main->getIndex());
   Abstract::Page::write(display, 70, 16, text, Font_16x26, Abstract::Page::White);

   display->DrawRect(115, 25, 120, 30, White, manualOffset);

   display->DrawRect(0, 50, 127, 63, White, true);
   const std::string deviceName = main->getDeviceName();
   const uint8_t deviceX = compileLeftX(63, deviceName, Font_6x8, Alignment::Center);
   Abstract::Page::write(display, deviceX, 53, deviceName, Font_6x8, Abstract::Page::Black);
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
