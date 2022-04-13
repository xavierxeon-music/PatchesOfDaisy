#include "Private/MidiRtMidiBase.h"

#include <QDebug>
#include <QProcess>

// port name

Midi::RtMidi::PortName::Map Midi::RtMidi::PortName::nameMap = generate();

QString Midi::RtMidi::PortName::makeNice(const QString& rawPortName)
{
    if (nameMap.contains(rawPortName))
      return nameMap.value(rawPortName);

   return rawPortName;
}

QString Midi::RtMidi::PortName::makeRaw(const QString& nicePortName)
{
   for (Map::const_iterator it = nameMap.constBegin(); it != nameMap.constEnd(); it++)
   {
      if (nicePortName == it.value())
         return it.key();
   }

   return nicePortName;
}

Midi::RtMidi::PortName::Map Midi::RtMidi::PortName::generate()
{
#if defined(__UNIX_JACK__)
   QProcess jackList;
   jackList.start("jack_lsp", {"--aliases", "--type", "midi"});
   if (!jackList.waitForFinished())
      return Map();

   const QString data = QString::fromUtf8(jackList.readAllStandardOutput());
   const QStringList lines = data.split('\n', Qt::SkipEmptyParts);

   Map map;
   QString systemName;
   for(QString line: qAsConst(lines))
   {
      line = line.trimmed();
      if(line.startsWith("system:"))
      {
         systemName = line;
         continue;
      }
      else if(line.startsWith("alsa_pcm:"))
      {
         QString name = line;
         name.remove("alsa_pcm:");
         name.replace("/midi", "");

         map[systemName] = name;
         continue;
      }
   }
   return map;
#else
   return Map();
#endif
}

// base

Midi::RtMidi::Base::Base(QObject* parent, const QString &portName)
   : QObject(parent)
   , portName(PortName::makeRaw(portName))
{
}

void Midi::RtMidi::Base::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qWarning() << "MIDI ERROR" << type << QString::fromStdString(errorText);
}
