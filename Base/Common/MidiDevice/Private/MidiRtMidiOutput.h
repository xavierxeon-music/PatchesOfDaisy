#ifndef MidiRtMidiOutputH
#define MidiRtMidiOutputH

#include <Midi/MidiInterfaceOutput.h>
#include <QObject>

#include "RtMidi4.h"

namespace Midi
{
   namespace RtMidi
   {
      class Output : public QObject, public Interface::Output
      {
         Q_OBJECT
      public:
         Output(QObject* parent, const QString& portName);
         virtual ~Output() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();

      protected:
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);

      protected:
         RtMidiOut output;
         const QString portName;

      private:
         void sendBuffer(const Bytes& buffer) override;
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiOutputH
