#ifndef MidiDeviceInputH
#define MidiDeviceInputH

#include "../../Private/MidiRtMidiInput.h"

namespace Midi
{
   namespace Device
   {
      // connect to an existing input
      class Input : public RtMidi::Input
      {
         Q_OBJECT
      public:
         Input(QObject* parent, const QString& portName);
         virtual ~Input() override;

      public:
         void open() override;
         void close() override;
      };
   } // namespace Device
} // namespace Midi

#endif // NOT MidiDeviceInputH
