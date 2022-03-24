#ifndef AbstractPageH
#define AbstractPageH

#include <Global.h>

#include <Abstract/Abstract.h>

#ifndef NON_DAISY_DEVICE

#include <daisy_patch.h>
using OledDisplay = daisy::OledDisplay<daisy::SSD130x4WireSpi128x64Driver>;

#else

#include <ImposterFontDef.h>
#include <ImposterOledDisplay.h>
using OledDisplay = Imposter::OledDisplay;
using FontDef = Imposter::FontDef;

#endif // NON_DAISY_DEVICE

namespace Abstract
{
   class Page
   {
   public:
      class Manager;
      class Overlay;
      class Base;

      static const bool White;
      static const bool Black;

      enum class Mode
      {
         Neutral,  // no highlight
         Select,   // select what to edit
         Operation // edit selected value
      };

      enum class State
      {
         Turn,   // turn pages with encoder
         Page,   // pass encoder to page
         Overlay // pass encoder to overlay and render overlay
      };

   protected:
      Page();

   public:
      virtual void render(OledDisplay* display);
      virtual State selected(); // when page is selected, before render
      virtual State forward();
      virtual State back();
      virtual State clicked();
      virtual State longPressed();
      virtual bool skip();

      // static, used by Abstract::Patch::renderSplash
      static void write(OledDisplay* display, const uint8_t x, const uint8_t y, const std::string& text, const FontDef& font, bool color);
      static uint8_t compileLeftX(const uint8_t x, const std::string& text, const FontDef& fontDef, const Alignment& alignment);

      const Mode& getMode() const;

   protected:
      virtual State turnIfNeutral() const = 0;

   protected:
      Mode mode;
   };
} // namespace Abstract

#endif //AbstractPageH
