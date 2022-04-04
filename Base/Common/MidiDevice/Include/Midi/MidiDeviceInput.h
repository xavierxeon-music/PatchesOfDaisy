#ifndef MidiDeviceInputH
#define MidiDeviceInputH

#include <Midi/MidiInterfaceInput.h>
#include <QObject>

#include "../RtMidi4/RtMidi4.h"

namespace Midi
{
   namespace Device
   {
      // connect to an existing input
      class Input : public QObject, public Interface::Input
      {
         Q_OBJECT
      public:
         Input(QObject* parent, const QString& inputPortName);
         virtual ~Input();

      public:
         void open() override;
         void close() override;
         static QStringList getAvailable();

      protected:
         RtMidiIn input;

      private:
         Q_INVOKABLE void dataFromInput(const Bytes& message) override;
         void openInput();
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
         static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

      private:
         const std::string inputPortName; // rt midi does not use QString
      };
   } // namespace Device
} // namespace Midi

#endif // NOT MidiDeviceInputH
