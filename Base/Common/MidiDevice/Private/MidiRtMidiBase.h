#ifndef MidiRtMidiBaseH
#define MidiRtMidiBaseH

#include <QObject>

#include "RtMidi4.h"

namespace Midi
{
   namespace RtMidi
   {
      class PortName
      {
      public:
         using Raw = QString;
         using Nice = QString;

      public:
         static Nice makeNice(const Raw& rawPortName);
         static Raw makeRaw(const Nice& nicePortName);

      private:
         using Map = QMap<Nice, Raw>;

      private:
         static Map generate();

      private:
         static Map nameMap;
      };

      class Base : public QObject
      {
         Q_OBJECT
      public:
         Base(QObject* parent);

      protected:
         static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
      };
   } // namespace RtMidi
} // namespace Midi

#endif // NOT MidiRtMidiBaseH
