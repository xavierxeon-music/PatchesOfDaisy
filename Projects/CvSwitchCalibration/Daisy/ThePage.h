#ifndef ThePageH
#define ThePageH

#include <Abstract/AbstractPageBase.h>

class CvSwitchCalibration;

class ThePage : public Abstract::Page::Base
{
public:
   ThePage(CvSwitchCalibration* main);

private:
   void render(OledDisplay* display) override;
   State selected() override;
   State forward() override;
   State back() override;
   State clicked() override;

private:
   CvSwitchCalibration* main;
};

#endif // ThePageH
