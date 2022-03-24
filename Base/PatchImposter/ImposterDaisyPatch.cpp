#include "ImposterDaisyPatch.h"

#include <QApplication>
#include <QThread>

#include <Abstract/AbstractPatch.h>

#include "MainWindow.h"

Imposter::DaisyPatch::DaisyPatch()
   : seed()
   , encoder()
   , controls()
   , gate_input()
   , display()
   , sampleRate(48000.0f)
   , audioBlockSize(48)
   , audioHandle(this)
   , mainWindow(nullptr)
{
   for(uint8_t channel = 0; channel < 4; channel++)
      controls[channel].init(channel);

   audioHandle.createBuffers();
}

void Imposter::DaisyPatch::Init()
{
   // do nothing
}

void Imposter::DaisyPatch::DelayMs(size_t del)
{
   QThread::msleep(static_cast<unsigned long>(del));
}

void Imposter::DaisyPatch::SetAudioSampleRate(SaiHandle::Config::SampleRate samplerate)
{
   if (SaiHandle::Config::SampleRate::SAI_8KHZ == samplerate)
      sampleRate = 8000.0f;
   else if (SaiHandle::Config::SampleRate::SAI_16KHZ == samplerate)
      sampleRate = 16000.0f;
   else if (SaiHandle::Config::SampleRate::SAI_32KHZ == samplerate)
      sampleRate = 32000.0f;
   else if (SaiHandle::Config::SampleRate::SAI_48KHZ == samplerate)
      sampleRate = 48000.0f;
   else if (SaiHandle::Config::SampleRate::SAI_96KHZ == samplerate)
      sampleRate = 96000.0f;
   else
      sampleRate = 0.0f;

   if (mainWindow)
      mainWindow->updateState();
}

float Imposter::DaisyPatch::AudioSampleRate()
{
   return sampleRate;
}

void Imposter::DaisyPatch::SetAudioBlockSize(size_t size)
{
   if (audioBlockSize == size)
      return;

   audioBlockSize = size;
   audioHandle.createBuffers();

   if (mainWindow)
      mainWindow->updateState();
}

size_t Imposter::DaisyPatch::AudioBlockSize()
{
   return audioBlockSize;
}

float Imposter::DaisyPatch::AudioCallbackRate()
{
   return sampleRate / static_cast<float>(audioBlockSize);
}

void Imposter::DaisyPatch::ProcessAllControls()
{
   if (mainWindow)
      mainWindow->externalControls();
}

void Imposter::DaisyPatch::startUiMain()
{
   QApplication::setApplicationName("ImposterDaisyPatch");

   static int argCount = 0;
   static const char* argValues[1] = {"ImposterDaisyPatch"};
   QApplication app(argCount, (char**)argValues);

   MainWindow mw(this);
   mw.show();

   mainWindow = &mw;
   app.exec();
   mainWindow = nullptr;
}

void Imposter::DaisyPatch::executeAudioStep()
{
   audioHandle.advanceInputBuffer();
}

void Imposter::DaisyPatch::executeNonAudioStep(bool updateDisplay)
{
   Abstract::Patch::the()->nonAudioLoop();

   if (updateDisplay)
      Abstract::Patch::the()->updateOled();
}

void Imposter::DaisyPatch::applyOutput(const Output& output)
{
   if (mainWindow)
      mainWindow->applyOutput(output);
}

QString Imposter::DaisyPatch::getAppName()
{
   const std::string appName = Abstract::Patch::the()->appName;
   return QString::fromStdString(appName);
}
