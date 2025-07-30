#include "PageFlameCC.h"

#include "MidiAccesories.h"

#ifndef NON_DAISY_DEVICE
const uint16_t PageFlameCC::portDuration = 1000;
#else
const uint16_t PageFlameCC::portDuration = 50;
#endif

PageFlameCC::PageFlameCC(MidiAccesories* main)
   : PageAbstract(main)
   , expander(main)
   , deltaTime(0)
   , port(16)
{
   updatePorts(true);
}

void PageFlameCC::render(OledDisplay* display)
{
   write(display, 10, 10, "Flame", Font_11x18, White);
   write(display, 10, 30, "u16MCC", Font_11x18, White);
   write(display, 10, 50, "click to init", Font_6x8, White);

   for (uint8_t index = 0; index < 16; index++)
   {
      const uint8_t x = (index < 8) ? 100 : 110;
      const uint8_t y = (index < 8) ? index * 8 : (index - 8) * 8;

      display->DrawRect(x, y, x + 8, y + 8, White, index == port.getCurrentValue());
   }
}

void PageFlameCC::advance(const uint16_t& msCallBackIntervall)
{
   deltaTime += msCallBackIntervall;
   if (portDuration > deltaTime)
      return;

   deltaTime -= portDuration;
   port.valueAndNext();

   updatePorts();
}

void PageFlameCC::initDevice()
{
   expander.sendSysEx();
}

void PageFlameCC::updatePorts(bool turnOffPrevious)
{
   const uint8_t current = port.getCurrentValue();
   const uint8_t previous = (0 == current) ? 15 : current - 1;

   if (turnOffPrevious)
      expander.setCV(previous, 0.0);

   expander.setCV(current, 1.0);
}
