#ifndef MidiPhysicalOutputH
#define MidiPhysicalOutputH

#include "../../Private/MidiRtMidiOutput.h"

namespace Midi
{
   namespace Physical
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
         bool isOpen() const;
      };
   } // namespace Physical
} // namespace Midi

#endif // NOT MidiPhysicalOutputH
