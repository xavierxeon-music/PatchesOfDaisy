#ifndef MidiInterfaceFlameCCHPP
#define MidiInterfaceFlameCCHPP

#include <Midi/MidiInterfaceFlameCC.h>
#include <Tools/Range.h>

// port pair

void Midi::Interface::FlameCC::NoteGatePair::noteOn(const Note& note)
{
   parent->setCV(pitchOutput, note.voltage / 5.0);
   parent->setCV(gateOutput, 1.0);
}

void Midi::Interface::FlameCC::NoteGatePair::noteOff(const Note& note)
{
   parent->setCV(pitchOutput, note.voltage / 5.0);
   parent->setCV(gateOutput, 0.0);
}

Midi::Interface::FlameCC::NoteGatePair::NoteGatePair(FlameCC* parent, uint8_t pitchOutput, uint8_t gateOutput)
   : parent(parent)
   , pitchOutput(pitchOutput)
   , gateOutput(gateOutput)
{
}

// interface

Midi::Interface::FlameCC::FlameCC(Handler::Base* midiHandler, const Channel& midiChannel)
   : midiHandler(midiHandler)
   , midiChannel(midiChannel)
{
}

void Midi::Interface::FlameCC::sendSysEx()
{
   Bytes buffer(38);

   buffer[0] = static_cast<uint8_t>(Midi::Event::SysExStart); // Exclusive Status
   buffer[1] = 0x7D; // Header Flame module
   buffer[2] = 0x0B; // Flame module “μ16MCC”
   buffer[3] = 0x01; // version 1
   buffer[4] = 0x06; // data type 1 (dump all data)

   for (uint8_t output = 0; output < 16; output++)
   {
      // MIDI channels for CV 16 outputs (range: 00=channel 1 .. 0F=channel 16)
      buffer[5 + output] = midiChannel;

      // control change numbers for 15 outputs
      const uint8_t message = static_cast<uint8_t>(ControllerMessage::User01) + output;
      buffer[21 + output] = message;
   }

   buffer[37] = static_cast<uint8_t>(Midi::Event::SysExStart); // End of Exclusive

   midiHandler->sendBuffer(buffer);
}

void Midi::Interface::FlameCC::setCV(uint8_t output, float value)
{
   const uint8_t message = ControllerMessage::User01 + Range::clamp<uint8_t>(output, 0, 15);
   const uint8_t iValue = Range::clamp<uint8_t>(value * 127, 0, 127);

   midiHandler->sendControllerChange(midiChannel, static_cast<ControllerMessage>(message), iValue);
}

Midi::Interface::FlameCC::NoteGatePair* Midi::Interface::FlameCC::createPair(uint8_t pitchOutput, uint8_t gateOutput)
{
   return new NoteGatePair(this, pitchOutput, gateOutput);
}

#endif // MidiInterfaceFlameCCHPP
