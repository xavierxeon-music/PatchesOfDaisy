#ifndef IndicatorBlockH
#define IndicatorBlockH

#include <Abstract/AbstractPage.h>

namespace Indicator
{
   class Block
   {
   public:
      inline Block(const uint8_t& x, const uint8_t& y, const uint8_t& size = 1);

   public:
      inline void render(OledDisplay* display, bool color);

   private:
      const uint8_t x;
      const uint8_t y;
      const uint8_t size;
   };

} // namespace Indicator

#include <IndicatorBlock.hpp>

#endif //  IndicatorBlockH
