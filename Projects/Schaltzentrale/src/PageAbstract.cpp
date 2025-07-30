#include "PageAbstract.h"

#include "Main.h"

PageAbstract::PageAbstract(Main* main)
   : Abstract::Page::Base()
   , main(main)
   , gateIndex(0)
   , operation(Operation::MinBar)
   , saveIndicator(1, 1)
   , blink(10)
{
}

void PageAbstract::renderHeader(OledDisplay* display, const std::string& text)
{
   blink.valueAndNext();

   display->DrawRect(0, 0, 128, 8, White, true);
   const uint8_t x = compileLeftX(64, text, Font_6x8, Alignment::Center);
   write(display, x, 1, text, Font_6x8, Page::Black);

   bool pending = Remember::Root::isUnsynced();
   saveIndicator.render(display, !pending);
}

bool PageAbstract::getEditColor(const Operation& editOperation, const uint8_t& editGateIndex) const
{
   if (Mode::Neutral == mode)
      return Page::White;

   if (editOperation != operation || editGateIndex != gateIndex)
      return Page::White;

   if (Mode::Operation == mode)
      return blink.value();

   return Page::Black;
}

bool PageAbstract::getBlinkColor() const
{
   return blink.value();
}

void PageAbstract::changeValue(bool up, const uint8_t offset)
{
   using Operator = void (RandomChain::*)(bool);
   using OperatorMap = std::map<Operation, Operator>;

   static const OperatorMap operatorMap = {{Operation::MinBar, &RandomChain::changeMinBarDuration},
                                           {Operation::MaxBar, &RandomChain::changeMaxBarDuration},
                                           {Operation::MinValue, &RandomChain::changeMinValue},
                                           {Operation::MaxValue, &RandomChain::changeMaxValue}};

   RandomChain* chain = main->getChain(gateIndex + offset);
   Operator func = operatorMap.at(operation);
   (chain->*func)(up);
}

Abstract::Page::State PageAbstract::clicked()
{
   if (Mode::Neutral == mode || Mode::Operation == mode)
   {
      mode = Mode::Select;
   }
   else if (Mode::Select == mode)
   {
      mode = Mode::Operation;
   }
   return turnIfNeutral();
}

Abstract::Page::State PageAbstract::longPressed()
{
   mode = Mode::Neutral;
   operation = Operation::MinBar;
   gateIndex = 0;

   return State::Overlay;
}
