#ifndef IndicatorPieH
#define IndicatorPieH

#include <Abstract/AbstractPage.h>

namespace Indicator
{
   class Pie
   {
   public:
      inline Pie(const uint8_t& x, const uint8_t& y, const uint8_t& quarterSize = 2);

   public:
      inline void render(OledDisplay* display, const float percentage, const bool color);

   private:
      struct TwoPoints
      {
         uint8_t x1;
         uint8_t y1;
         uint8_t x2;
         uint8_t y2;
      };

      using IndexMap = std::map<uint8_t, TwoPoints>;

   private:
      const uint8_t x;
      const uint8_t y;
      const uint8_t quarterSize;
      IndexMap indexMap;
   };
} // namespace Indicator

#include <IndicatorPie.hpp>

#endif //  IndicatorPieH
