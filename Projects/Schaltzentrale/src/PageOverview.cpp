#include "PageOverview.h"

#include <Tools/Text.h>
#include <Tools/Variable.h>

#include "Main.h"

const std::vector<PageOverview::Operation> PageOverview::operationOrder = {Operation::Gate1, Operation::Gate2, Operation::Gate3, Operation::Gate4, Operation::Ramp1, Operation::Ramp2};

PageOverview::PageOverview(Main* main)
   : PageAbstract(main)
{
}

void PageOverview::render(OledDisplay* display)
{
   renderHeader(display, "Overview");

   for (uint8_t i = 0; i < 6; i++)
   {
      auto getEditColor = [&]() -> bool
      {
         if (Mode::Neutral == mode)
            return Page::White;

         if (operationOrder.at(i) != operation)
            return Page::White;

         if (Mode::Operation == mode)
            return getBlinkColor();

         return Page::Black;
      };

      if (i < 4)
      {
         const uint8_t yGate = 15 + i * 13;
         RandomChain* chainGate = main->getChain(i);

         const float linkPercentage = 1.0 - chainGate->getCurrentLinkPrecentage(0.0);
         const uint8_t length = 20 * linkPercentage;
         display->DrawRect(5, yGate + 1, 5 + length, yGate + 5, White, chainGate->isOn());

         if (Mode::Neutral == mode)
         {
            const std::string currentText = Text::convert(chainGate->getCurrentBarDuration());
            const uint8_t currentTextX = compileLeftX(50, currentText, Font_7x10, Alignment::Right);
            write(display, currentTextX, yGate, currentText, Font_7x10, Page::White);
         }
         else
         {
            const std::string modeText = (RandomChain::Type::RndGate == chainGate->getType()) ? "RND" : "ALT";
            const uint8_t modeTextX = compileLeftX(50, modeText, Font_7x10, Alignment::Right);
            write(display, modeTextX, yGate, modeText, Font_7x10, getEditColor());
         }
      }
      else
      {
         const uint8_t yRamp = 15 + (i - 4) * 26;
         RandomChain* chainRamp = main->getChain(i);

         // duration
         const float linkPercentage = 1.0 - chainRamp->getCurrentLinkPrecentage(0.0);
         const uint8_t length = 40 * linkPercentage;
         display->DrawRect(55, yRamp + 1, 55 + length, yRamp + 3, White, true);

         // value
         const std::string valText = Text::convert(chainRamp->linkStartValue()) + ">" + Text::convert(chainRamp->linkEndValue());
         write(display, 55, yRamp + 8, valText, Font_6x8, Page::White);

         const float valuePercentage = chainRamp->getValue(0.0) / 255.0;
         const uint8_t valueLength = 40 * valuePercentage;
         display->DrawRect(55, yRamp + 17, 55 + valueLength, yRamp + 19, White, false);

         if (Mode::Neutral == mode)
         {
            // duration
            const std::string currentText = Text::convert(chainRamp->getCurrentBarDuration());
            const uint8_t currentTextX = compileLeftX(125, currentText, Font_7x10, Alignment::Right);
            write(display, currentTextX, yRamp, currentText, Font_7x10, Page::White);

            // value
            const std::string currentValText = Text::convert(chainRamp->getValue(0.0));
            const uint8_t currentValTextX = compileLeftX(125, currentValText, Font_7x10, Alignment::Right);
            write(display, currentValTextX, yRamp + 12, currentValText, Font_7x10, Page::White);
         }
         else
         {
            const std::string modeText = (RandomChain::Type::Ramp == chainRamp->getType()) ? "RMP" : "STD";
            const uint8_t modeTextX = compileLeftX(125, modeText, Font_7x10, Alignment::Right);
            write(display, modeTextX, yRamp, modeText, Font_7x10, getEditColor());
         }
      }
   }
}

Abstract::Page::State PageOverview::forward()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.increment();
   }
   else if (Mode::Operation == mode)
   {
      alterChainType();
   }
   return turnIfNeutral();
}

Abstract::Page::State PageOverview::back()
{
   if (Mode::Select == mode)
   {
      Variable::Enum<Operation> varOperation(operation, operationOrder, true);
      varOperation.decrement();
   }
   else if (Mode::Operation == mode)
   {
      alterChainType();
   }
   return turnIfNeutral();
}

void PageOverview::alterChainType() const
{
   std::vector<Operation>::const_iterator it = std::find(operationOrder.cbegin(), operationOrder.cend(), operation);
   if (it == operationOrder.cend())
      return;

   uint8_t index = it - operationOrder.cbegin();
   RandomChain* chain = main->getChain(index);

   const RandomChain::Type type = chain->getType();

   if (RandomChain::Type::Ramp == type)
      chain->setType(RandomChain::Type::Steady);
   else if (RandomChain::Type::Steady == type)
      chain->setType(RandomChain::Type::Ramp);
   else if (RandomChain::Type::RndGate == type)
      chain->setType(RandomChain::Type::AltGate);
   else if (RandomChain::Type::AltGate == type)
      chain->setType(RandomChain::Type::RndGate);

   Remember::Root::setUnsynced();
}
