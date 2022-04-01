#include "MidiTunnelClient.h"

Midi::Tunnel::Client::Client(QObject* parent, const QString& serverName)
   : Socket(parent)
   , serverName(serverName)
{
}
