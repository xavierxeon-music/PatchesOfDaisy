#ifndef AudioChannelInputH
#define AudioChannelInputH

#include "AudioChannelAbstract.h"

#include <QLabel>
#include <QPixmap>

#include <Music/Note.h>
#include <Sound/StandardTable.h>
#include <Sound/WaveTableOscilator.h>

namespace Imposter
{
   class DaisyPatch;
}

namespace AudioChannel
{
   class Input : public Abstract
   {
      Q_OBJECT
   public:
      Input();

   public:
      void init(Imposter::DaisyPatch* imposterPatch);
      Buffer process(const uint64_t& blockSize);
      void setup(QLabel* oscilatorLabel, QPixmap* pixmap);
      void setRemote(const Note& newNote, const Standard::Waveform::Shape& newWaveform);

   private:
      void updateOscilator();
      using Abstract::setup; // make base class function private

   private:
      Imposter::DaisyPatch* imposterPatch;
      QLabel* oscilatorLabel;
      float frequency;
      Standard::Waveform::Shape waveform;
      Standard::Table table;
      WaveTable::Oscilator oscilator;
   };
} // namespace AudioChannel

#endif // AudioChannelInputH
