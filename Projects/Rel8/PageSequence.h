#ifndef PageSequenceH
#define PageSequenceH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>
#include <Tools/SquareSignal.h>

class RelativeEight;

class PageSequence : public Abstract::Page::Base
{
public:
   PageSequence(RelativeEight* main);

private:
   enum class SeqeunceOperation
   {
      NumberOfSteps,
      StepSize,
      NoteSelect
   };

private:
   void render(OledDisplay* display) override;
   void renderKeyboard(OledDisplay* display);
   void renderSequence(OledDisplay* display);
   State forward() override;
   State back() override;
   State clicked() override;

private:
   RelativeEight* main;
   SeqeunceOperation operation;
   SquareSignal blink;
   Indicator::Block saveIndicator;
   uint8_t editStepIndex;
};

#endif // PageSequenceH
