#include "Icons.h"

void Icons::render(OledDisplay* display, const Type& type, const uint8_t x, const uint8_t& y, bool color)
{
   if (ArrowDown == type)
      BitMask<12, 12>::render(display, x, y, dataArrowDown, !color);
   else if (ArrowUp == type)
      BitMask<12, 12>::render(display, x, y, dataArrowUp, !color);
   else if (ChannelActive == type)
      BitMask<12, 12>::render(display, x, y, dataChannelActive, !color);
   else if (ChannelInactive == type)
      BitMask<12, 12>::render(display, x, y, dataChannelInactive, !color);
   else if (Pause == type)
      BitMask<12, 12>::render(display, x, y, dataPause, !color);
}
