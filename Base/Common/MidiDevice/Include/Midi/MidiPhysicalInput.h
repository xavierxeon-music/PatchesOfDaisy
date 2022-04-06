#ifndef MidiPhysicalInputH
#define MidiPhysicalInputH

#include "../../Private/MidiRtMidiInput.h"

namespace Midi
{
   namespace Physical
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
   } // namespace Physical
} // namespace Midi

#endif // NOT MidiPhysicalInputH
