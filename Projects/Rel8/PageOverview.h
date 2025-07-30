#ifndef PageOverviewH
#define PageOverviewH

#include <Abstract/AbstractPageBase.h>

#include <Music/Tempo.h>

class RelativeEight;

class PageOverview : public Abstract::Page::Base
{
public:
   PageOverview(RelativeEight* main, const Tempo& tempo);

private:
   void render(OledDisplay* display) override;
   void renderChannel(OledDisplay* display, const uint8_t& channelIndex);
   State forward() override;
   State back() override;
   State clicked() override;

private:
   RelativeEight* main;
   const Tempo& tempo;
};

#endif // PageOverviewH
