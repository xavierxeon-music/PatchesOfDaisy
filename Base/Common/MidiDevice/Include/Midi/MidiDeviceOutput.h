#ifndef MidiDeviceOutputH
#define MidiDeviceOutputH

#include "../../Private/MidiRtMidiOutput.h"

namespace Midi
{
   namespace Device
   {
      // connect to an existing output
      class Output : public RtMidi::Output
      {
         Q_OBJECT
      public:
         Output(QObject* parent, const QString& portName);
         virtual ~Output();

      public:
         void open() override;
         void close() override;
      };
   } // namespace Device
} // namespace Midi

#endif // NOT MidiDeviceOutputH
