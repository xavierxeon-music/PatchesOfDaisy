#ifndef MidiVirtualInputH
#define MidiVirtualInputH

#include <Midi/MidiInterfaceInput.h>
#include <QObject>

#include "../RtMidi4/RtMidi4.h"

namespace Midi
{
   namespace Virtual
   {
      // creates a virtual midi input, ignoires passthrough
      class Input : public QObject, public Interface::Input
      {
         Q_OBJECT
      public:
         Input(QObject* parent, const QString& inputPortName);
         virtual ~Input();

      public:
         void open() override;
         void close() override;

      protected:
         RtMidiIn input;

      private:
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
         static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

      private:
         const QString inputPortName;
         bool isOpen;
      };
   } // namespace Virtual
} // namespace Midi

#endif // NOT MidiVirtualInputH
