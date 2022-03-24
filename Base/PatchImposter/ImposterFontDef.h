#ifndef ImposterFontDefH
#define ImposterFontDefH

#include <Global.h>

namespace Imposter
{
   struct FontDef
   {
      const uint8_t FontWidth;
      uint8_t FontHeight;
      const uint16_t* data;
   };

} // namespace Imposter

extern Imposter::FontDef Font_6x8;
extern Imposter::FontDef Font_7x10;
extern Imposter::FontDef Font_11x18;
extern Imposter::FontDef Font_16x26;

#endif // ImposterFontDefH
