#ifndef IconsH
#define IconsH

#include <Graphics/BitMask.h>

class Icons
{
public:
   enum Type
   {
      ArrowDown,
      ArrowUp,
      ChannelActive,
      ChannelInactive,
      Pause
   };

public:
   static void render(OledDisplay* display, const Type& type, const uint8_t x, const uint8_t& y, bool color);

private:
   static const BitMask<12, 12>::Data dataArrowDown;
   static const BitMask<12, 12>::Data dataArrowUp;
   static const BitMask<12, 12>::Data dataChannelActive;
   static const BitMask<12, 12>::Data dataChannelInactive;
   static const BitMask<12, 12>::Data dataPause;
};

#endif // IconsH
