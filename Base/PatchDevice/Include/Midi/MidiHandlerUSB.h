#ifndef MidiHandlerUSBH
#define MidiHandlerUSBH

#include <Midi/MidiHandlerBase.h>

#ifndef NON_DAISY_DEVICE
using MidiUsbHandler = daisy::MidiUsbHandler;
#else
#include <ImposterMidiUsbHandler.h>
using MidiUsbHandler = Imposter::MidiUsbHandler;
#endif // NON_DAISY_DEVICE

namespace Midi
{
   namespace Handler
   {
      class USB : public Base
      {
      public:
         inline USB(Base* passThroughHandler = nullptr);

      public:
         inline void initMidi() override;
         inline void pollMidiReceive() override final;

         inline bool settingsUpdate(Remember::Root* root, const Midi::ControllerMessage& message, const uint8_t& value);

      private:
         inline void sendBuffer(const Bytes& buffer) override final;

      private:
         MidiUsbHandler midi2; // called midi2 to avoid potential clashes with internal handler
         Bytes receivedDataBase64;
         Bytes toBeSentDataBase64;
         size_t sendIndex;
      };
   } // namespace Handler
} // namespace Midi

#include "MidiHandlerUSB.hpp"

#endif // MidiHandlerUSBH
