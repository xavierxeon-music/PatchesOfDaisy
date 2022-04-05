#ifndef MidiHandlerInternalH
#define MidiHandlerInternalH

#include <Midi/MidiHandlerBase.h>

#include <Abstract/AbstractPatch.h>

namespace Midi
{
   namespace Handler
   {
      class Internal : public Base
      {
      public:
         inline Internal(DaisyPatch* daisy, Base* passThroughHandler, const Channel& receiveChannel = 8);

      public:
         inline void initMidi();
         inline void pollMidiReceive() override final;

      private:
         inline void sendBuffer(const Bytes& buffer) override final;

      private:
#ifndef NON_DAISY_DEVICE
         daisy::MidiUartHandler& midi1; // reference to daisy internal midi handler
#endif // NON_DAISY_DEVICE
      };
   } // namespace Handler
} // namespace Midi

#ifndef MidiHandlerInternalHPP
#include "MidiHandlerInternal.hpp"
#endif // NOT MidiHandlerInternalHPP

#endif // MidiHandlerInternalH
