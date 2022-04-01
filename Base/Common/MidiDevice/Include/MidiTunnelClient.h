#ifndef MidiTunnelClientH
#define MidiTunnelClientH

#include "../Private/MidiTunnelSocket.h"

namespace Midi
{
   namespace Tunnel
   {
      class Client : public Socket
      {
         Q_OBJECT
      public:
         Client(QObject* parent, const QString& serverName);

      private:
         const QString serverName;
      };
   } // namespace Tunnel
} // namespace Midi

#endif // NOT MidiTunnelClientH
