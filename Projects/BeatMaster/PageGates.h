#ifndef PageGatesH
#define PageGatesH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>
#include <Music/Tempo.h>
#include <Tools/Range.h>
#include <Tools/SquareSignal.h>

class Main;

class PageGates : public Abstract::Page::Base
{
public:
   PageGates(Main* main, const Tempo& tempo);

private:
   enum class GateOperation
   {
      Channel,
      Knobs,
      Wait,
      Top,
      Zero,
      Step
   };

private:
   void render(OledDisplay* display) override;
   void renderGate(OledDisplay* display, const uint8_t index);
   State forward() override;
   State back() override;
   State clicked() override;
   void changeChannel(bool up);

private:
   Main* main;
   const Tempo& tempo;
   Indicator::Block saveIndicator;
   GateOperation operation;
   uint8_t audioChannel;
   SquareSignal blink;
   Range::Mapper percentMapper;
};

#endif // PageGatesH
