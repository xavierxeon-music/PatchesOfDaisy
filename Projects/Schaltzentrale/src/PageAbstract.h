#ifndef PageAbstractH
#define PageAbstractH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>
#include <Utility/SquareSignal.h>

class RandomChain;
class Main;

class PageAbstract : public Abstract::Page::Base
{
public:
   PageAbstract(Main* main);

protected:
   enum class Operation
   {
      MinBar,
      MaxBar,
      MinValue,
      MaxValue
   };

protected:
   void renderHeader(OledDisplay* display, const std::string& text);
   bool getEditColor(const Operation& editOperation, const uint8_t& editGateIndex) const;
   bool getBlinkColor() const;
   void changeValue(bool up, const uint8_t offset = 0);

protected:
   Main* main;
   uint8_t gateIndex;
   Operation operation;

private:
   State clicked() override;
   State longPressed() override;

private:
   Indicator::Block saveIndicator;
   SquareSignal blink;
};

#endif // NOT PageAbstractH
