#ifndef AbstractPageManagerH
#define AbstractPageManagerH

#include <Abstract/AbstractPageBase.h>

namespace Abstract
{
   class Page::Manager
   {
   public:
      Manager();

   public:
      void addPage(Base* basePage);
      void render(OledDisplay* display);
      void forward();
      void back();
      void clicked();
      void longPressed();

   private:
      Page::Base::Vector pageList;
      size_t index;    // of current page
      Page::State state;
   };
} // namespace Abstract

#endif // AbstractPageManagerH
