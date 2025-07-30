#ifndef PageFlameCCH
#define PageFlameCCH

#include "PageAbstract.h"

#include <Midi/MidiInterfaceFlameCC.h>
#include <Tools/Counter.h>

class MidiAccesories;

class PageFlameCC : public PageAbstract
{
public:
   PageFlameCC(MidiAccesories* main);

private:
   void render(OledDisplay* display) override;
   void advance(const uint16_t& msCallBackIntervall) override;
   void initDevice() override;

private:
   void updatePorts(bool turnOffPrevious = false);

private:
   Midi::Interface::FlameCC expander;
   uint16_t deltaTime;
   Counter port;
   static const uint16_t portDuration;
};

#endif // PageFlameCCH
