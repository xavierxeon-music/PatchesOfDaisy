#include "PageOverview.h"

PageOverview::PageOverview()
   : Abstract::Page::Base()
{
}

void PageOverview::render(OledDisplay* display)
{
   write(display, 5, 5, "Mesh", Font_16x26, White);
}

Abstract::Page::State PageOverview::clicked()
{
   return State::Turn;
}
