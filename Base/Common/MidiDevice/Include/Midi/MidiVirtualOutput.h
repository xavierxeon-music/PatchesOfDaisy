#ifndef MidiVirtualOutputH
#define MidiVirtualOutputH

#include <Midi/MidiInterfaceOutput.h>

#include "../RtMidi4/RtMidi4.h"

namespace Midi
{
   namespace Virtual
   {
      // creates a virtual midi output
      class Output : public Interface::Output
      {
      public:
         Output(const QString& outputPortName);
         virtual ~Output();

      public:
         void open() override;
         void close() override;

      protected:
         RtMidiOut output;

      private:
         void sendBuffer(const Bytes& buffer) override;
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);

      private:
         const QString outputPortName;
         bool isOpen;
      };
   } // namespace Virtual
} // namespace Midi

#endif // NOT MidiVirtualOutputH
