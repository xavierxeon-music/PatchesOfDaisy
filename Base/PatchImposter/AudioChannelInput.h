#ifndef AudioChannelInputH
#define AudioChannelInputH

#include "AudioChannelAbstract.h"

#include <QLabel>
#include <QPixmap>

#include <Music/Note.h>
#include <Sound/StandardTable.h>
#include <Sound/WaveTable.h>

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
      void updateOscilator(bool init);
      using Abstract::setup; // make base class function private

   private:
      Imposter::DaisyPatch* imposterPatch;
      QLabel* oscilatorLabel;
      Note note;
      Standard::Waveform::Shape waveform;
      WaveTable::Oscilator oscilator;
   };
} // namespace AudioChannel

#endif // AudioChannelInputH
