#include "AudioChannelInput.h"

#include "ImposterDaisyPatch.h"

AudioChannel::Input::Input()
   : Abstract()
   , imposterPatch(nullptr)
   , oscilatorLabel(nullptr)
   , note(Note::zeroNote)
   , waveform(Standard::Waveform::Sine)
   , table()
   , oscilator()
{
   oscilator.setAmplitude(1.0);
}

void AudioChannel::Input::init(Imposter::DaisyPatch* imposterPatch)
{
   this->imposterPatch = imposterPatch;
   oscilator.init(&table, imposterPatch->AudioSampleRate());
   updateOscilator();
}

std::vector<float> AudioChannel::Input::process(const uint64_t& blockSize)
{
   Buffer values(blockSize, 0.0);
   for (uint64_t index = 0; index < blockSize; index++)
      values[index] = oscilator.createSound();

   updatePixmap(values);
   return values;
}

void AudioChannel::Input::setup(QLabel* oscilatorLabel, QPixmap* pixmap)
{
   Abstract::setup(pixmap);
   this->oscilatorLabel = oscilatorLabel;

   updateOscilator();
}

void AudioChannel::Input::setRemote(const Note& newNote, const Standard::Waveform::Shape& newWaveform)
{
   if (waveform != newWaveform)
   {
      waveform = newWaveform;
      updateOscilator();
   }

   if (note.frequency != newNote.frequency)
   {
      note = newNote;
      updateOscilator();
   }
}

void AudioChannel::Input::updateOscilator()
{
   table.setWaveform(waveform);
   oscilator.setFrequency(note.frequency);

   if (oscilatorLabel)
   {
      const std::string text = note.name + " [" + Standard::Waveform::getName(waveform) + "]";
      oscilatorLabel->setText(QString::fromStdString(text));
   }
}
