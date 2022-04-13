#include "Private/MidiRtMidiBase.h"

#include <QDebug>
#include <QProcess>

// port name

QString Midi::RtMidi::PortName::makeNice(const QString& rawPortName)
{
   return "HELLO_" + rawPortName;
}

QString Midi::RtMidi::PortName::makeRaw(const QString& sequencerName)
{
   return sequencerName.mid(6);
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
