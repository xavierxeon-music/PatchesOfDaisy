#ifndef SequencePageAbstractH
#define SequencePageAbstractH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>
#include <Tools/Range.h>
#include <Tools/SquareSignal.h>

class Main;
class Rhythm;

class PageAbstract : public Abstract::Page::Base
{
public:
   PageAbstract(Main* main);

protected:
   void renderSide(OledDisplay* display, const bool lowerScreen = false);
   Rhythm& getCurrentRhythm() const;

protected:
   Main* main;
   Range::Mapper seqHeightMapper;
   SquareSignal blink;

private:
   Indicator::Block saveIndicator;
};

#endif // SequencePageAbstractH
