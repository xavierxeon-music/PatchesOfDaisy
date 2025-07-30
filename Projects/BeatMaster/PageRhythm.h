#ifndef SequencePageRhythmH
#define SequencePageRhythmH

#include "PageAbstract.h"

class PageRhythm : public PageAbstract
{
public:
   PageRhythm(Main* main);

private:
   enum class RhythmOperation
   {
      NumberOfSteps,
      StepSize,
      Velocity
   };

private:
   void render(OledDisplay* display) override;
   void renderChannel(OledDisplay* display);
   void renderSettings(OledDisplay* display);
   void renderSequence(OledDisplay* display);
   State forward() override;
   State back() override;
   State clicked() override;

private:
   RhythmOperation operation;
   uint8_t currentStep;
};

#endif // SequencePageRhythmH
