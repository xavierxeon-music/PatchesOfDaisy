#include "PageOverview.h"

#include "RelativeEight.h"

PageOverview::PageOverview(RelativeEight* main, const Tempo& tempo)
   : Abstract::Page::Base()
   , main(main)
   , tempo(tempo)
{
}

void PageOverview::render(OledDisplay* display)
{
   renderChannel(display, 0);
   renderChannel(display, 1);
}

void PageOverview::renderChannel(OledDisplay* display, const uint8_t& channelIndex)
{
   const uint8_t yStart = 32 * channelIndex;
   Channel* channel = main->getChannel(channelIndex);

   display->DrawRect(0, yStart, 128, yStart + 8, White, true);
   write(display, 0, yStart + 1, channel->rootNote.name, Font_6x8, Black);
   const uint8_t selectedChannel = main->getChannel(channelIndex)->selection;
   write(display, 30, yStart + 1, Channel::names.at(selectedChannel), Font_6x8, Black);

   Sequence* sequence = main->getSequence(selectedChannel);

   const uint8_t maxIndex = sequence->getNumberOfSteps();
   const uint8_t currentIndex = sequence->currentIndex();
   for (uint8_t index = 0; index < maxIndex; index++)
   {
      const uint8_t x = index * 15;

      const bool even = (0 == (index % 2));
      const uint8_t yOffset = even ? 12 : 22;

      const bool color = (index == currentIndex) ? Black : White;

      const uint8_t stepValue = sequence->getStepValue(index);
      if (Sequence::pauseValue == stepValue)
      {
         write(display, x, yStart + yOffset, "X", Font_6x8, color);
      }
      else
      {
         const uint8_t midiValue = channel->rootNote.midiValue + stepValue;
         const Note note = Note::fromMidi(midiValue);
         write(display, x, yStart + yOffset, note.name, Font_6x8, color);
      }
   }
}

Abstract::Page::State PageOverview::forward()
{
   return turnIfNeutral();
}

Abstract::Page::State PageOverview::back()
{
   return turnIfNeutral();
}

Abstract::Page::State PageOverview::clicked()
{
   return turnIfNeutral();
}
