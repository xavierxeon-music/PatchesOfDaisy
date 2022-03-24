#ifndef IndicatorSliderHPP
#define IndicatorSliderHPP

#include <Graphics/IndicatorSlider.h>

Indicator::Slider::Slider(const uint8_t& x, const uint8_t& y, const uint8_t& width, const uint8_t height)
   : x(x)
   , y(y)
   , width(width)
   , height(height)
   , percentageMapper(0.0, 1.0, 2.0, width - 2)
{
}

void Indicator::Slider::render(OledDisplay* display, const float percentage, const bool color)
{
   // background
   display->DrawRect(x, y, x + width, y + height, !color, true);

   // lane
   display->DrawRect(x + 2, y + 2, x + (width - 2), y + height - 2, color, false);

   // tick
   const uint indicatorX = x + static_cast<uint8_t>(percentageMapper(percentage));
   display->DrawRect(indicatorX - 1, y, indicatorX + 1, y + height, color, true);
}

#endif // IndicatorSliderHPP
