#ifndef MidiRtMidiOutputH
#define MidiRtMidiOutputH

#include "MidiRtMidiBase.h"
#include <Midi/MidiInterfaceOutput.h>

namespace Midi
{
   namespace RtMidi
   {
      class Output : public Base, public Interface::Output
      {
         Q_OBJECT
      public:
         Output(QObject* parent, const QString& portName);
         virtual ~Output() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();
         void sendBuffer(const Bytes& buffer) override;

      protected:
         RtMidiOut output;
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiOutputH
