#ifndef ThePageH
#define ThePageH

#include <Abstract/AbstractPageBase.h>

class TestThingsHere;

class ThePage : public Abstract::Page::Base
{
public:
   ThePage(TestThingsHere* main);

private:
   void render(OledDisplay* display) override;
   State selected() override;
   State forward() override;
   State back() override;
   State clicked() override;

private:
   TestThingsHere* main;
};

#endif // ThePageH
