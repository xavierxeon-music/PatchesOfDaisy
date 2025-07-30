#include "PageRamp.h"

#include <Blocks/RandomChain.h>
#include <Tools/Text.h>
#include <Tools/Variable.h>

#include "Main.h"

const std::vector<PageRamp::Operation> PageRamp::operationOrder = {Operation::MinBar, Operation::MaxBar, Operation::MinValue, Operation::MaxValue};

PageRamp::PageRamp(Main* main)
   : PageAbstract(main)
{
}

void PageRamp::render(OledDisplay* display)
{
   renderHeader(display, "Ramps");

   for (uint8_t i = 0; i < 2; i++)
   {
      const uint8_t y = 15 + i * 26;
      RandomChain* chain = main->getChain(i + 4);

      // duration
      const std::string minDurText = Text::convert(chain->getMinBarDuration());
      const uint8_t minDurTextX = compileLeftX(25, minDurText, Font_7x10, Alignment::Right);
      bool colorMinDurText = getEditColor(Operation::MinBar, i);
      write(display, minDurTextX, y, minDurText, Font_7x10, colorMinDurText);

      const std::string maxDurText = Text::convert(chain->getMaxBarDuration());
      const uint8_t maxDurTextX = compileLeftX(50, maxDurText, Font_7x10, Alignment::Right);
      bool colorMaxDurText = getEditColor(Operation::MaxBar, i);
      write(display, maxDurTextX, y, maxDurText, Font_7x10, colorMaxDurText);

      const float linkPercentage = 1.0 - chain->getCurrentLinkPrecentage(0.0);
      const uint8_t length = 40 * linkPercentage;
      display->DrawRect(55, y + 1, 55 + length, y + 3, White, true);

      const std::string currentText = Text::convert(chain->getCurrentBarDuration());
      const uint8_t currentTextX = compileLeftX(125, currentText, Font_7x10, Alignment::Right);
      write(display, currentTextX, y, currentText, Font_7x10, Page::White);

      // value
      const std::string minValText = Text::convert(chain->getMinValue());
      const uint8_t minValTextX = compileLeftX(25, minValText, Font_7x10, Alignment::Right);
      bool colorMinValText = getEditColor(Operation::MinValue, i);
      write(display, minValTextX, y + 12, minValText, Font_7x10, colorMinValText);

      const std::string maxValText = Text::convert(chain->getMaxValue());
      const uint8_t maxValTextX = compileLeftX(50, maxValText, Font_7x10, Alignment::Right);
      bool colorMaxValText = getEditColor(Operation::MaxValue, i);
      write(display, maxValTextX, y + 12, maxValText, Font_7x10, colorMaxValText);

      const std::string valText = Text::convert(chain->linkStartValue()) + ">" + Text::convert(chain->linkEndValue());
      write(display, 55, y + 8, valText, Font_6x8, Page::White);

      const float valuePercentage = chain->getValue(0.0) / 255.0;
      const uint8_t valueLength = 40 * valuePercentage;
      display->DrawRect(55, y + 17, 55 + valueLength, y + 19, White, false);

      const std::string currentValText = Text::convert(chain->getValue(0.0));
      const uint8_t currentValTextX = compileLeftX(125, currentValText, Font_7x10, Alignment::Right);
      write(display, currentValTextX, y + 12, currentValText, Font_7x10, Page::White);
   }
}

Abstract::Page::State PageRamp::forward()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.increment();

      if (Operation::MinBar == operation)
      {
         Variable::Integer<uint8_t> varIndex(gateIndex, 0, 1, true);
         varIndex.increment();
      }
   }
   else if (Mode::Operation == mode)
   {
      changeValue(true, 4);
   }
   return turnIfNeutral();
}

Abstract::Page::State PageRamp::back()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.decrement();

      if (Operation::MaxValue == operation)
      {
         Variable::Integer<uint8_t> varIndex(gateIndex, 0, 1, true);
         varIndex.decrement();
      }
   }
   else if (Mode::Operation == mode)
   {
      changeValue(false, 4);
   }
   return turnIfNeutral();
}
