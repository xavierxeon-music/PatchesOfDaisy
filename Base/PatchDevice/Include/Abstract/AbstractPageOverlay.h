#ifndef AbstractPageOverlayH
#define AbstractPageOverlayH

#include <Abstract/AbstractPage.h>

namespace Abstract
{
   class Page::Overlay : public Page
   {
   public:
      static const uint8_t offset;
      static const uint8_t width;
      static const uint8_t height;

   public:
      virtual State forward() override;
      virtual State back() override;
      virtual State clicked() override;
      virtual State longPressed() override;

   protected:
      Overlay();

   protected:
      void drawFrame(OledDisplay* display, bool color);
      State turnIfNeutral() const override final;
   };
} // namespace Abstract

#endif //  AbstractPageOverlayH
