#include <Abstract/AbstractPageOverlay.h>

const uint8_t Abstract::Page::Overlay::offset = 5;
const uint8_t Abstract::Page::Overlay::width = 118;
const uint8_t Abstract::Page::Overlay::height = 54;

Abstract::Page::Overlay::Overlay()
   : Page()
{
}

void Abstract::Page::Overlay::drawFrame(OledDisplay* display, bool color)
{
   static const uint8_t x1 = offset - 1;
   static const uint8_t y1 = offset - 1;
   static const uint8_t x2 = offset + width + 1;
   static const uint8_t y2 = offset + height + 1;

   display->DrawRect(x1, y1, x2, y2, !color, true);

   display->DrawLine(x1, y1, x1, y2, color);
   display->DrawLine(x2, y1, x2, y2, color);

   display->DrawLine(x1, y1, x2, y1, color);
   display->DrawLine(x1, y2, x2, y2, color);
}

Abstract::Page::State Abstract::Page::Overlay::forward()
{
   // do nothing
   return State::Overlay;
}

Abstract::Page::State Abstract::Page::Overlay::back()
{
   // do nothing
   return State::Overlay;
}

Abstract::Page::State Abstract::Page::Overlay::clicked()
{
   // do nothing
   return State::Overlay;
}

Abstract::Page::State Abstract::Page::Overlay::longPressed()
{
   // do nothing
   return State::Turn;
}

Abstract::Page::State Abstract::Page::Overlay::turnIfNeutral() const
{
   if (Mode::Neutral != mode)
      return State::Overlay;
   else
      return State::Turn;
}
