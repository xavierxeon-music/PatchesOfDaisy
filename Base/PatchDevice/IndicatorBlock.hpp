#ifndef IndicatorBlockHPP
#define IndicatorBlockHPP

#include <Graphics/IndicatorBlock.h>

Indicator::Block::Block(const uint8_t& x, const uint8_t& y, const uint8_t& size)
   : x(x)
   , y(y)
   , size(size)
{
}

void Indicator::Block::render(OledDisplay* display, bool color)
{
   display->DrawRect(x, y, x + size, y + size, color, true);
}

#endif // IndicatorBlockHPP
