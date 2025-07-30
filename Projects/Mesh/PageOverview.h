#ifndef PageOverviewH
#define PageOverviewH

#include <Abstract/AbstractPageBase.h>


class PageOverview : public Abstract::Page::Base
{
public:
   PageOverview();

private:
   void render(OledDisplay* display) override;
   State clicked() override;

private:
};

#endif // PageOverviewH
