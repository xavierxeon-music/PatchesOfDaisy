#ifndef MidiHandlerInternalHPP
#define MidiHandlerInternalHPP

#include <Midi/MidiHandlerInternal.h>

Midi::Handler::Internal::Internal(DaisyPatch* daisy, const Channel& receiveChannel, Base* passThroughHandler)
   : Base(receiveChannel, passThroughHandler)
#ifndef NON_DAISY_DEVICE
   , midi1(daisy->midi)
#endif // NON_DAISY_DEVICE
{
   (void)daisy;
}

void Midi::Handler::Internal::initMidi(bool verbose)
{
   (void)verbose;
#ifndef NON_DAISY_DEVICE
   // already configured
   midi1.StartReceive();
#endif // NON_DAISY_DEVICE
}

void Midi::Handler::Internal::pollMidiReceive()
{
#ifndef NON_DAISY_DEVICE
   midi1.Listen();
   while (midi1.HasEvents())
   {
      MidiEvent event = midi1.PopEvent();
      handleMidiMessage(event);
   }
#endif // NON_DAISY_DEVICE
}

void Midi::Handler::Internal::sendBuffer(const Bytes& buffer)
{
#ifndef NON_DAISY_DEVICE
   midi1.SendMessage(const_cast<uint8_t*>(buffer.data()), buffer.size());
#else
   (void)sizeof(buffer); // to silence warnings
#endif // NON_DAISY_DEVICE
}

#endif // MidiHandlerInternalHPP
