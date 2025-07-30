#include "OverlayCv.h"

#include "CustomTable.h"

OverlayCv::OverlayCv(CustomTable* table)
   : Abstract::Page::Overlay()
   , cvMapping(table->getCvMapping())
   , selectedChannel(0)
   , selectedType(CvMapping::Pitch)
   , blink(10)
{
}

void OverlayCv::render(OledDisplay* display)
{
   drawFrame(display, White);

   const bool currentColor = blink.valueAndNext();

   static const uint8_t channelX[4] = {50, 70, 90, 110};

   auto drawRow = [&](const uint8_t y, const std::string title, const CvMapping::Type& type)
   {
      write(display, 10, y, title, Font_6x8, White);

      for (uint8_t channel = 0; channel < 4; channel++)
      {
         if (channel == selectedChannel && type == selectedType)
         {
            display->DrawRect(channelX[channel] - 1, y, channelX[channel] + 10 + 1, y + 6, !currentColor, false);
         }
         display->DrawRect(channelX[channel], y + 1, channelX[channel] + 10, y + 5, White, cvMapping->get(type, channel));
      }
   };

   drawRow(15, "pitch", CvMapping::Pitch);
   drawRow(30, "blend", CvMapping::Blend);
   drawRow(45, "seed", CvMapping::Seed);
}

Abstract::Page::State OverlayCv::forward()
{
   selectedChannel++;
   if (4 == selectedChannel)
   {
      selectedChannel = 0;

      if (CvMapping::Pitch == selectedType)
         selectedType = CvMapping::Blend;
      else if (CvMapping::Blend == selectedType)
         selectedType = CvMapping::Seed;
      else if (CvMapping::Seed == selectedType)
         selectedType = CvMapping::Pitch;
   }

   return State::Overlay;
}

Abstract::Page::State OverlayCv::back()
{
   if (0 == selectedChannel)
   {
      selectedChannel = 3;

      if (CvMapping::Pitch == selectedType)
         selectedType = CvMapping::Seed;
      else if (CvMapping::Blend == selectedType)
         selectedType = CvMapping::Pitch;
      else if (CvMapping::Seed == selectedType)
         selectedType = CvMapping::Blend;
   }
   else
   {
      selectedChannel--;
   }
   return State::Overlay;
}

Abstract::Page::State OverlayCv::clicked()
{
   const bool value = cvMapping->get(selectedType, selectedChannel);
   cvMapping->set(selectedType, selectedChannel, !value);

   return State::Overlay;
}
