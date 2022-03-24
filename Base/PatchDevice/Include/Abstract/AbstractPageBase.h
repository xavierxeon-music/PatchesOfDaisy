#ifndef AbstractPageBaseH
#define AbstractPageBaseH

#include <Abstract/AbstractPage.h>

namespace Abstract
{
   class Page::Base : public Page
   {
   public:
      using Vector = std::vector<Base*>;

      static const uint8_t width;
      static const uint8_t height;

   protected:
      Base();

   public:
      void setOverlay(Overlay* overlay);
      Overlay* getOverlay();

   protected:
      State turnIfNeutral() const override final;

   private:
      Overlay* overlay;
   };
} // namespace Abstract

#endif //AbstractPageBaseH
