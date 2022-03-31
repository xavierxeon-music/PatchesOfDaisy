#ifndef MidiHandlerBaseH
#define MidiHandlerBaseH

#include <Midi/MidiInterface.h>

#ifndef NON_DAISY_DEVICE
#include <daisy_patch.h>
using MidiEvent = daisy::MidiEvent;
#else
#include <ImposterMidiEvent.h>
using MidiEvent = Imposter::MidiEvent;
#endif // NON_DAISY_DEVICE

namespace Midi
{
   namespace Handler
   {
      class Base : public Interface
      {

      public:
         inline Base(const Channel& receiveChannel, Base* passThroughHandler);

      protected:
         virtual void pollMidiReceive() = 0; // IF you want to receive midi, then call this function periodically, fx. in nonAudioLoop
         inline void handleMidiMessage(MidiEvent event);

      private:
         const Channel& receiveChannel;
         Base* passThroughHandler;
      };
   } // namespace Handler
} // namespace Midi

#ifndef MidiHandlerBaseHPP
#include "MidiHandlerBase.hpp"
#endif // NOT MidiHandlerBaseHPP

#endif // MidiHandlerBaseH
