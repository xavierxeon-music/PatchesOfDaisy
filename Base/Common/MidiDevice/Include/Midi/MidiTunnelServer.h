#ifndef MidiTunnelServerH
#define MidiTunnelServerH

#include "../Private/MidiTunnelSocket.h"

#include <QTcpServer>

namespace Midi
{
   namespace Tunnel
   {
      // tunnel an input and output interface via tcp
      class Server : public Socket
      {
         Q_OBJECT
      public:
         Server(QObject* parent);

      private slots:
         void slotHasNewConnection();

      private:
         QTcpServer* server;
      };
   } // namespace Tunnel
} // namespace Midi

#endif // NOT MidiTunnelServerH
