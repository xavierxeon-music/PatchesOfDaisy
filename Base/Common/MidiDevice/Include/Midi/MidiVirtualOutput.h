#ifndef MidiVirtualOutputH
#define MidiVirtualOutputH

#include "../../Private/MidiRtMidiOutput.h"

namespace Midi
{
   namespace Virtual
   {
      // creates a virtual midi output
      class Output : public RtMidi::Output
      {
         Q_OBJECT
      public:
         Output(QObject* parent, const QString& portName);
         virtual ~Output();

      public:
         void open() override;
         void close() override;

      private:
         bool isOpen;
      };
   } // namespace Virtual
} // namespace Midi

#endif // NOT MidiVirtualOutputH
