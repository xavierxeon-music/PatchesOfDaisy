#ifndef OverlayCvH
#define OverlayCvH

#include <Abstract/AbstractPageOverlay.h>

#include <Utility/SquareSignal.h>

#include "CvMapping.h"

class CustomTable;

class OverlayCv : public Abstract::Page::Overlay
{
public:
   OverlayCv(CustomTable* table);

public:
   void render(OledDisplay* display) override;
   State forward() override;
   State back() override;
   State clicked() override;

private:
   CvMapping* cvMapping;
   uint8_t selectedChannel;
   CvMapping::Type selectedType;
   SquareSignal blink;
};

#endif //  OverlayCvH
