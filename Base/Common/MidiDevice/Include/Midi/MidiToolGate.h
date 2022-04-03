#ifndef MidiToolGateH
#define MidiToolGateH

#include <Midi/MidiCommon.h>
#include <Midi/MidiInterfaceInput.h>

namespace Midi
{
   namespace Tool
   {
      class Gate
      {
      public:
         Gate(Interface::Input* input, const Channel& channel, const Note& note);

      public:
         bool isOn() const;

      private:
         void noteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity);
         void noteOff(const Midi::Channel& channel, const Note& note);

      private:
         const Channel myChannel;
         const Note myNote;

         bool on;
      };
   } // namespace Tool
} // namespace Midi

#endif // NOT MidiToolGateH
