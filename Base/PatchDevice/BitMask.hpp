#ifndef BitMaskHPP
#define BitMaskHPP

#include <Graphics/BitMask.h>

template <uint8_t Width, uint8_t Height>
void BitMask<Width, Height>::render(OledDisplay* display, const uint8_t x, const uint8_t& y, const Data& data, bool invert)
{
   for (uint8_t xData = 0; xData < Width; xData++)
   {
      const uint8_t xScreen = x + xData;
      for (uint8_t yData = 0; yData < Height; yData++)
      {
         const uint8_t yScreen = y + yData;
         const bool pixel = data[xData][yData];
         display->DrawPixel(xScreen, yScreen, invert ? !pixel : pixel);
      }
   }
}

#endif // BitMaskHPP
