#include <Abstract/AbstractPageManager.h>

#include <Abstract/AbstractPageOverlay.h>

Abstract::Page::Manager::Manager()
   : pageList()
   , index(0)
   , state(Page::State::Turn)
{

}

void Abstract::Page::Manager::addPage(Base* basePage)
{
   pageList.push_back(basePage);
}

void Abstract::Page::Manager::render(OledDisplay* display)
{
   if(pageList.empty())
      return;

   pageList[index]->render(display);

   if (Page::State::Overlay == state && pageList[index]->getOverlay())
      pageList[index]->getOverlay()->render(display);
}

void Abstract::Page::Manager::forward()
{
   if (pageList.empty())
      return;

   if (Page::State::Turn == state)
   {
      if (pageList.size() == index + 1)
         index = 0;
      else
         index++;

      if (pageList[index]->skip())
         forward();
      else
         state = pageList[index]->selected();
   }
   else if (Page::State::Overlay == state && pageList[index]->getOverlay())
   {
      state = pageList[index]->getOverlay()->forward();
   }
   else
   {
      state = pageList[index]->forward();
   }
}

void Abstract::Page::Manager::back()
{
   if (pageList.empty())
      return;

   if (Page::State::Turn == state)
   {
      if (0 == index)
         index = pageList.size() - 1;
      else
         index--;

      if (pageList[index]->skip())
         back();
      else
         state = pageList[index]->selected();
   }
   else if (Page::State::Overlay == state && pageList[index]->getOverlay())
   {
      state = pageList[index]->getOverlay()->back();
   }
   else
   {
      state = pageList[index]->back();
   }
}

void Abstract::Page::Manager::clicked()
{
   if(pageList.empty())
      return;

   if (Page::State::Overlay == state && pageList[index]->getOverlay())
      state = pageList[index]->getOverlay()->clicked();
   else
      state = pageList[index]->clicked();
}

void Abstract::Page::Manager::longPressed()
{
   if (pageList.empty())
      return;

   if (Page::State::Turn == state && pageList[index]->getOverlay())
   {
      state = Page::State::Overlay;
      pageList[index]->getOverlay()->selected();
   }
   else if (Page::State::Overlay == state && pageList[index]->getOverlay())
   {
      state = pageList[index]->getOverlay()->longPressed();
   }
   else
   {
      state = pageList[index]->longPressed();
   }
}
