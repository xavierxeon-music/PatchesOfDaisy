#include "PageChannel.h"

#include <Music/Note.h>
#include <Sound/WaveTable.h>
#include <Tools/Text.h>

#include "CustomTable.h"
#include "OverlayCv.h"
#include "RandomWalkTables.h"
#include "WaveBlender.h"

const Range::Mapper PageChannel::amplitudeMapper(-1.0, 1.0, 50.0, 2.0);
const Range::Mapper PageChannel::seedMapper(0, RandomWalkTables::maxSeed, 0.0, 127.0);
const Range::Mapper PageChannel::blendMapper(0, 1.0, 50.0, 2.0);

PageChannel::PageChannel(CustomTable* table, WaveTable::Oscilator* oscilator, uint8_t channel)
   : Abstract::Page::Base()
   , table(table)
   , oscilator(oscilator)
   , channel(channel)
   , operation(WaveOperation::NoteOffset)
   , saveIndicator(0, 61)
{
   setOverlay(new OverlayCv(table));
}

void PageChannel::render(OledDisplay* display)
{
   const bool graphColor = White;

   static const float maxAngle = 4.0f * Maths::pi;
   static const float anglePerStep = maxAngle / static_cast<float>(width);

   uint8_t lastY = 0;
   for (uint8_t x = 0; x < width; x++)
   {
      const float angle = x * anglePerStep;
      const float& value = table->valueByAngle(angle);
      const uint8_t y = static_cast<uint8_t>(amplitudeMapper(value));

      if (0 != x)
         display->DrawLine(x - 1, lastY, x, y, graphColor);

      lastY = y;
   }

   const uint8_t& seed = seedMapper(table->getMappedSeed());
   const uint8_t& blend = blendMapper(table->getMappedBlend());

   // lines
   if (WaveOperation::Blend == operation)
   {
      display->DrawLine(0, blend, 127, blend, graphColor);
   }
   else if (WaveOperation::Seed == operation)
   {
      display->DrawLine(seed, 2, seed, 50, graphColor);
   }
   else
   {
      for (uint8_t y = 2; y < 50; y += 4)
         display->DrawPixel(seed, y, graphColor);

      for (uint8_t x = 0; x < width; x += 4)
         display->DrawPixel(x, blend, graphColor);
   }

   renderFooter(display);
}

void PageChannel::renderFooter(OledDisplay* display)
{
   const bool textColor = White;

   // footer
   const uint8_t pageIndicatorWidth = (Mode::Neutral == mode) ? 15 : 5;
   for (uint8_t c = 0; c < 4; c++)
   {
      const uint8_t x = 5 + c * pageIndicatorWidth;
      if (c == channel)
         display->DrawRect(x + 1, 54, x + pageIndicatorWidth, 62, textColor, true);
      else
         display->DrawRect(x + 1, 54, x + pageIndicatorWidth, 62, textColor, false);
   }

   if (Mode::Neutral == mode)
   {
      const float frequency = oscilator->getFrequency();
      const std::string freqText = Text::convert(frequency, 0) + "Hz";
      const uint8_t freqY = 115 - (freqText.size() * 8);
      write(display, freqY, 55, freqText, Font_6x8, textColor);

      const Note note = Note::fromFrequency(frequency);
      write(display, 110, 55, note.name, Font_6x8, textColor);
   }
   else if (WaveOperation::NoteOffset == operation)
   {
      write(display, 32, 55, "C0 in >", Font_6x8, textColor);

      const Note offsetNote = table->getOffsetNote();
      write(display, 80, 55, offsetNote.name + " out", Font_6x8, textColor);
   }
   else if (WaveOperation::Waveform == operation)
   {
      write(display, 50, 55, "wave", Font_6x8, textColor);
      write(display, 80, 55, table->getWaveformName(), Font_6x8, textColor);
   }
   else if (WaveOperation::Blend == operation)
   {
      write(display, 44, 55, "blend", Font_6x8, textColor);

      const std::string blendText = Text::convert(table->getBlend());
      write(display, 80, 55, blendText, Font_6x8, textColor);
   }
   else if (WaveOperation::Seed == operation)
   {
      write(display, 50, 55, "seed", Font_6x8, textColor);

      const std::string seedText = Text::convert(table->getSeed());
      write(display, 80, 55, seedText, Font_6x8, textColor);
   }

   bool pending = Remember::Root::isUnsynced();
   saveIndicator.render(display, (Mode::Select == mode) ? !pending : pending);
}

Abstract::Page::State PageChannel::forward()
{
   if (Mode::Operation == mode)
   {
      if (WaveOperation::NoteOffset == operation)
         table->changeOffsetNote(true);
      else if (WaveOperation::Waveform == operation)
         table->changeWaveform(true);
      else if (WaveOperation::Blend == operation)
         table->changeBlend(true);
      else if (WaveOperation::Seed == operation)
         table->changeSeed(true);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageChannel::back()
{
   if (Mode::Operation == mode)
   {
      if (WaveOperation::NoteOffset == operation)
         table->changeOffsetNote(false);
      else if (WaveOperation::Waveform == operation)
         table->changeWaveform(false);
      else if (WaveOperation::Blend == operation)
         table->changeBlend(false);
      else if (WaveOperation::Seed == operation)
         table->changeSeed(false);
   }

   return turnIfNeutral();
}

Abstract::Page::State PageChannel::clicked()
{
   if (Mode::Neutral == mode)
   {
      operation = WaveOperation::NoteOffset;
      mode = Mode::Operation;
   }
   else
   {
      if (WaveOperation::NoteOffset == operation)
         operation = WaveOperation::Waveform;
      else if (WaveOperation::Waveform == operation)
         operation = WaveOperation::Blend;
      else if (WaveOperation::Blend == operation)
         operation = WaveOperation::Seed;
      else if (WaveOperation::Seed == operation)
      {
         operation = WaveOperation::Waveform;
         mode = Mode::Neutral;
      }
   }

   return turnIfNeutral();
}

Abstract::Page::State PageChannel::longPressed()
{
   mode = Mode::Neutral;
   operation = WaveOperation::NoteOffset;

   return State::Overlay;
}
