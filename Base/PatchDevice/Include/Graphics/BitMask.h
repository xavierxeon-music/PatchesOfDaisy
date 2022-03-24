#ifndef BitMaskH
#define BitMaskH

#include <Abstract/AbstractPage.h>

template <uint8_t Width, uint8_t Height>
struct BitMask
{
public:
   using Data = bool[Width][Height];

public:
   inline static void render(OledDisplay* display, const uint8_t x, const uint8_t& y, const Data& data, bool invert = false);
};

#include <BitMask.hpp>

#endif //  BitMaskH
