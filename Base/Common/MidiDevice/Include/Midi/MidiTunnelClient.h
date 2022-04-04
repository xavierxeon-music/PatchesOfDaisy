#ifndef MidiTunnelClientH
#define MidiTunnelClientH

#include "../Private/MidiTunnelSocket.h"

namespace Midi
{
   namespace Tunnel
   {
      // tunnel an input and output interface via tcp
      class Client : public Socket
      {
         Q_OBJECT
      public:
         Client(QObject* parent, const QString& serverName);

      private slots:
         void slotTryToConnectToHost();

      private:
         const QString serverName;
      };
   } // namespace Tunnel
} // namespace Midi

#endif // NOT MidiTunnelClientH
