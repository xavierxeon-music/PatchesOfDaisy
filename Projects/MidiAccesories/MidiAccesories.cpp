#include "MidiAccesories.h"

#include "PageFlameCC.h"

MidiAccesories::MidiAccesories()
   : Abstract::Patch("MidiAccesories")
   , Midi::Handler::Internal(&daisy, 1, nullptr)
   , deviceList()
{
   auto addPage = [&](PageAbstract* page)
   {
      deviceList.push_back(page);
      pageManager.addPage(page);
   };

   addPage(new PageFlameCC(this));
}

void MidiAccesories::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;
   (void)out;
   (void)size;

   const float msCallBackIntervall = 1000.0f / daisy.AudioCallbackRate();
   for (PageAbstract* device : deviceList)
      device->advance(msCallBackIntervall);

   // input
   const Encoder encoder = compileEncoder(true);
   updatePageManager(encoder);
}

// main function

int main(void)
{
   MidiAccesories app;
   app.run();
}
