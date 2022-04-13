#include "Private/MidiRtMidiBase.h"

#include <QDebug>
#include <QProcess>

// port name

Midi::RtMidi::PortName::Map Midi::RtMidi::PortName::nameMap = generate();

QString Midi::RtMidi::PortName::makeNice(const QString& rawPortName)
{
   for (Map::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      if (rawPortName == it.value())
         return it.key();
   }
   return rawPortName;
}

QString Midi::RtMidi::PortName::makeRaw(const QString& nicePortName)
{
   if (nameMap.contains(nicePortName))
      return nameMap.value(nicePortName);

   return nicePortName;
}

Midi::RtMidi::PortName::Map Midi::RtMidi::PortName::generate()
{
#if defined(__UNIX_JACK__)
   QProcess jackList;
   jackList.start("jack_lsp", {"--aliases", "--type", "midi"});
   if (!jackList.waitForFinished())
      return Map();

   qDebug() << jackList.readAllStandardOutput();
   return Map();
#else
   return Map();
#endif
}

// base

Midi::RtMidi::Base::Base(QObject* parent)
   : QObject(parent)
{
}

void Midi::RtMidi::Base::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qWarning() << "MIDI ERROR" << type << QString::fromStdString(errorText);
}
