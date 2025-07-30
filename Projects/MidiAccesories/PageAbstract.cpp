#include "PageAbstract.h"

PageAbstract::PageAbstract(Midi::Handler::Base* midiHandler)
   : Abstract::Page::Base()
   , midiHandler(midiHandler)
{
}

void PageAbstract::advance(const uint16_t& msCallBackIntervall)
{
   (void)msCallBackIntervall;
   // do nothing
}

void PageAbstract::initDevice()
{
   // do nothing
}

Abstract::Page::State PageAbstract::clicked()
{
   initDevice();
   return State::Turn;
}
