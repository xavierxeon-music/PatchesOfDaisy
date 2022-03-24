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
         inline Internal(DaisyPatch* daisy, const Channel& receiveChannel, Base* passThroughHandler);

      public:
         inline void initMidi() override;
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

#include "MidiHandlerInternal.hpp"

#endif // MidiHandlerInternalH
