#ifndef MidiRtMidiInputH
#define MidiRtMidiInputH

#include "MidiRtMidiBase.h"
#include <Midi/MidiInterfaceInput.h>

namespace Midi
{
   namespace RtMidi
   {
      class Input : public Base, public Interface::Input
      {
         Q_OBJECT
      public:
         Input(QObject* parent, const QString& portName);
         virtual ~Input() = 0;

      public:
         virtual void open() override = 0;
         virtual void close() override = 0;
         static QStringList getAvailable();

      protected:
         static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

      protected:
         RtMidiIn input;

      private:
         Q_INVOKABLE void dataFromInput(const Bytes& message) override;
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiInputH
