#ifndef PageChannelH
#define PageChannelH

#include <Abstract/AbstractPageBase.h>

#include <Graphics/IndicatorBlock.h>
#include <Tools/Range.h>

class CustomTable;

namespace WaveTable
{
   class Oscilator;
}

class PageChannel : public Abstract::Page::Base
{
public:
   PageChannel(CustomTable* table, WaveTable::Oscilator* oscilator, uint8_t channel);

private:
   enum class WaveOperation
   {
      NoteOffset,
      Waveform,
      Blend,
      Seed
   };

private:
   void render(OledDisplay* display) override;
   void renderFooter(OledDisplay* display);
   State forward() override;
   State back() override;
   State clicked() override;
   State longPressed() override;

private:
   CustomTable* table;
   WaveTable::Oscilator* oscilator;
   uint8_t channel;
   WaveOperation operation;
   Indicator::Block saveIndicator;
   static const Range::Mapper amplitudeMapper;
   static const Range::Mapper seedMapper;
   static const Range::Mapper blendMapper;
};

#endif // PageChannelH
