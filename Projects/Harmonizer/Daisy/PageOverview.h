#ifndef PageOverviewH
#define PageOverviewH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>

#include "Harmonizer.h"

class PageOverview : public Abstract::Page::Base
{
public:
   PageOverview(Harmonizer* main);

private:
   void render(OledDisplay* display) override;
   State selected() override;
   State forward() override;
   State back() override;

private:
   Harmonizer* main;
   Indicator::Block saveIndicator;
};

#endif // PageOverviewH
