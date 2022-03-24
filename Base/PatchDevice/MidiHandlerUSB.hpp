#ifndef MidiHandlerUSBHPP
#define MidiHandlerUSBHPP

#include <Midi/MidiHandlerUSB.h>

#include <Tools/SevenBit.h>

Midi::Handler::USB::USB(Base* passThroughHandler)
   : Base(0, passThroughHandler)
   , midi2()
   , receivedDataBase64()
   , toBeSentDataBase64()
   , sendIndex(0)
{
}

void Midi::Handler::USB::initMidi()
{
#ifndef NON_DAISY_DEVICE
   daisy::MidiUsbHandler::Config midi_config;
   midi_config.transport_config.periph = daisy::MidiUsbTransport::Config::INTERNAL;
   midi2.Init(midi_config);
#endif // NON_DAISY_DEVICE

   midi2.StartReceive();
}

void Midi::Handler::USB::pollMidiReceive()
{
   midi2.Listen();
   while (midi2.HasEvents())
   {
      MidiEvent event = midi2.PopEvent();
      handleMidiMessage(event);
   }

   if (toBeSentDataBase64.empty())
      return;

   if (sendIndex == toBeSentDataBase64.size())
   {
      toBeSentDataBase64.clear();

      Bytes message(3);
      message[0] = (Midi::Event::ControlChange | 0); // control change @ channel 1
      message[1] = Midi::ControllerMessage::RememberApply;
      message[2] = 0;
      sendBuffer(message);
   }
   else
   {
      Bytes message(3);
      message[0] = (Midi::Event::ControlChange | 0); // control change @ channel 1
      message[1] = Midi::ControllerMessage::RememberBlock;
      message[2] = toBeSentDataBase64.at(sendIndex);
      sendBuffer(message);

      sendIndex++;
   }
}

bool Midi::Handler::USB::settingsUpdate(Remember::Root* root, const Midi::ControllerMessage& message, const uint8_t& value)
{
   if (!root)
      return false;

   if (Midi::ControllerMessage::RememberBlock == message)
   {
      receivedDataBase64 << value;
      return false;
   }
   else if (Midi::ControllerMessage::RememberApply == message)
   {
      Remember::DataVector data = SevenBit::decode(receivedDataBase64);
      receivedDataBase64.clear();

      root->set(data);
      root->setUnsynced();

      return true;
   }
   else if (Midi::ControllerMessage::RememberRequest == message)
   {
      if (toBeSentDataBase64.empty())
      {
         Remember::DataVector data = root->get();
         toBeSentDataBase64 = SevenBit::encode(data);
         sendIndex = 0;
      }

      return false;
   }

   return false; // to silence warning, should never pass here
}

void Midi::Handler::USB::sendBuffer(const Bytes& buffer)
{
   midi2.SendMessage(const_cast<uint8_t*>(buffer.data()), buffer.size());
}

#endif // MidiHandlerUSBHPP
