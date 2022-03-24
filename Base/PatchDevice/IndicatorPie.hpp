#ifndef IndicatorPieHPP
#define IndicatorPieHPP

#include <cmath>

#include <Graphics/IndicatorPie.h>

Indicator::Pie::Pie(const uint8_t& x, const uint8_t& y, const uint8_t& quarterSize)
   : x(x)
   , y(y)
   , quarterSize(quarterSize)
   , indexMap()
{
   // subSquares @ index + 1
   //  4 | 4 | 5 | 6
   // ---------------
   //  3 | 3 | 5 | 6
   // ---------------
   //  2 | 1 | 7 | 7
   // ---------------
   //  2 | 1 | 8 | 8

   auto subX = [&](const uint8_t index) -> uint8_t
   {
      return static_cast<uint8_t>(x + (index * quarterSize));
   };

   auto subY = [&](const uint8_t index) -> uint8_t
   {
      return static_cast<uint8_t>(y + (index * quarterSize));
   };

   indexMap[0] = {subX(1), subY(2), subX(1), subY(3)};
   indexMap[1] = {subX(0), subY(2), subX(0), subY(3)};
   indexMap[2] = {subX(0), subY(1), subX(1), subY(1)};
   indexMap[3] = {subX(0), subY(0), subX(1), subY(0)};

   indexMap[4] = {subX(2), subY(0), subX(2), subY(1)};
   indexMap[5] = {subX(3), subY(0), subX(3), subY(1)};
   indexMap[6] = {subX(2), subY(2), subX(3), subY(2)};
   indexMap[7] = {subX(2), subY(3), subX(3), subY(3)};
}

void Indicator::Pie::render(OledDisplay* display, const float percentage, const bool color)
{
   uint maxIndex = 0;
   while (maxIndex < 9)
   {
      const float centerValue = maxIndex * 0.125;
      if (percentage > centerValue - 0.0625 && percentage < centerValue + 0.0625)
         break;

      maxIndex++;
   }

   for (uint8_t index = 0; index < maxIndex; index++)
   {
      const TwoPoints& tp = indexMap[index];
      display->DrawRect(tp.x1, tp.y1, tp.x1 + quarterSize, tp.y1 + quarterSize, color, true);
      display->DrawRect(tp.x2, tp.y2, tp.x2 + quarterSize, tp.y2 + quarterSize, color, true);
   }

   display->DrawRect(x, y, x + (4 * quarterSize), y + (4 * quarterSize), color, false);
}

#endif // IndicatorPieHPP
