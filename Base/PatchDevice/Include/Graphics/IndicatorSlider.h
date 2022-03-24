#ifndef IndicatorSliderH
#define IndicatorSliderH

#include <Abstract/AbstractPage.h>

#include <Tools/Range.h>

namespace Indicator
{
   class Slider
   {
   public:
      inline Slider(const uint8_t& x, const uint8_t& y, const uint8_t& width, const uint8_t height = 6);

   public:
      inline void render(OledDisplay* display, const float percentage, const bool color);

   private:
      const uint8_t x;
      const uint8_t y;
      const uint8_t width;
      const uint8_t height;
      const Range::Mapper percentageMapper;
   };
} // namespace Indicator

#include <IndicatorSlider.hpp>

#endif //  IndicatorSliderH
