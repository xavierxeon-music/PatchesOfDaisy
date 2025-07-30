#ifndef PageOverviewH
#define PageOverviewH

#include "PageAbstract.h"

#include <Graphics/IndicatorBlock.h>
#include <Music/Tempo.h>
#include <Tools/SquareSignal.h>

class TimeLord;

class PageOverview : public PageAbstract
{
public:
   PageOverview(Main* main, const Tempo& tempo);

private:
   void render(OledDisplay* display) override;
   void renderSequences(OledDisplay* display);
   State forward() override;
   State back() override;
   State clicked() override;

private:
   const Tempo& tempo;
};

#endif // PageOverviewH
