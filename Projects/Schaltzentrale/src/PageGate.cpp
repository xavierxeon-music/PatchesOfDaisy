#include "PageGate.h"

#include <Blocks/RandomChain.h>
#include <Tools/Text.h>
#include <Tools/Variable.h>

#include "Main.h"

const std::vector<PageGate::Operation> PageGate::operationOrder = {Operation::MinBar, Operation::MaxBar};

PageGate::PageGate(Main* main)
   : PageAbstract(main)
{
}

void PageGate::render(OledDisplay* display)
{
   renderHeader(display, "Gates");

   for (uint8_t i = 0; i < 4; i++)
   {
      const uint8_t y = 15 + i * 13;
      RandomChain* chain = main->getChain(i);

      const float linkPercentage = 1.0 - chain->getCurrentLinkPrecentage(0.0);
      const uint8_t length = 20 * linkPercentage;
      display->DrawRect(5, y + 1, 5 + length, y + 5, White, chain->isOn());

      const std::string currentText = Text::convert(chain->getCurrentBarDuration());
      const uint8_t currentTextX = compileLeftX(50, currentText, Font_7x10, Alignment::Right);
      write(display, currentTextX, y, currentText, Font_7x10, White);

      const std::string minText = Text::convert(chain->getMinBarDuration());
      const uint8_t minTextX = compileLeftX(85, minText, Font_7x10, Alignment::Right);
      bool colorMinText = getEditColor(Operation::MinBar, i);
      write(display, minTextX, y, minText, Font_7x10, colorMinText);

      write(display, 85, y, " / ", Font_7x10, White);

      const std::string maxText = Text::convert(chain->getMaxBarDuration());
      const uint8_t maxTextX = compileLeftX(125, maxText, Font_7x10, Alignment::Right);
      bool colorMaxText = getEditColor(Operation::MaxBar, i);
      write(display, maxTextX, y, maxText, Font_7x10, colorMaxText);
   }
}

Abstract::Page::State PageGate::forward()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.increment();

      if (Operation::MinBar == operation)
      {
         Variable::Integer<uint8_t> varIndex(gateIndex, 0, 3, true);
         varIndex.increment();
      }
   }
   else if (Mode::Operation == mode)
   {
      changeValue(true);
   }
   return turnIfNeutral();
}

Abstract::Page::State PageGate::back()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.decrement();

      if (Operation::MaxBar == operation)
      {
         Variable::Integer<uint8_t> varIndex(gateIndex, 0, 3, true);
         varIndex.decrement();
      }
   }
   else if (Mode::Operation == mode)
   {
      changeValue(false);
   }
   return turnIfNeutral();
}
