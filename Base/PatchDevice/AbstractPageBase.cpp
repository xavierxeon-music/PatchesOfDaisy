#include <Abstract/AbstractPageBase.h>

const uint8_t Abstract::Page::Base::width = 128;
const uint8_t Abstract::Page::Base::height = 64;

Abstract::Page::Base::Base()
   : Page()
   , overlay(nullptr)
{
}

void Abstract::Page::Base::setOverlay(Overlay* overlay)
{
   this->overlay = overlay;
}

Abstract::Page::Overlay* Abstract::Page::Base::getOverlay()
{
   return overlay;
}

Abstract::Page::State Abstract::Page::Base::turnIfNeutral() const
{
   if (Mode::Neutral != mode)
      return State::Page;
   else
      return State::Turn;
}
