#ifndef MidiRtMidiInputH
#define MidiRtMidiInputH

#include <Midi/MidiInterfaceInput.h>
#include <QObject>

#include "RtMidi4.h"

namespace Midi
{
   namespace RtMidi
   {
      class Input : public QObject, public Interface::Input
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
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
         static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

      protected:
         RtMidiIn input;
         const QString portName;

      private:
         Q_INVOKABLE void dataFromInput(const Bytes& message) override;
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiInputH
