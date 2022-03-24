#include "AudioChannelInput.h"

#include "ImposterDaisyPatch.h"

AudioChannel::Input::Input()
   : Abstract()
   , imposterPatch(nullptr)
   , oscilatorLabel(nullptr)
   , note(Note::zeroNote)
   , waveform(Standard::Waveform::Sine)
   , oscilator()
{
   oscilator.setAmplitude(1.0);
}

void AudioChannel::Input::init(Imposter::DaisyPatch* imposterPatch)
{
   this->imposterPatch = imposterPatch;
   updateOscilator(true);
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

   updateOscilator(false);
}

void AudioChannel::Input::setRemote(const Note& newNote, const Standard::Waveform::Shape& newWaveform)
{
   if (waveform != newWaveform)
   {
      waveform = newWaveform;
      updateOscilator(true);
   }

   if (note.frequency != newNote.frequency)
   {
      note = newNote;
      updateOscilator(false);
   }
}

void AudioChannel::Input::updateOscilator(bool init)
{
   if (init)
      oscilator.init(Standard::getTable(waveform), imposterPatch->AudioSampleRate());

   oscilator.setFrequency(note.frequency);

   if (oscilatorLabel)
   {
      const std::string text = note.name + " [" + Standard::getName(waveform) + "]";
      oscilatorLabel->setText(QString::fromStdString(text));
   }
}
