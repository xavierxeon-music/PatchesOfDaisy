#ifndef MidiTunnelSocketH
#define MidiTunnelSocketH

#include <Midi/MidiInterface.h>

#include <QPointer>
#include <QTcpSocket>

namespace Midi
{
   namespace Tunnel
   {
      class Socket : public QObject, public Interface
      {
         Q_OBJECT
      public:
         static const int port;

      public:
         Socket(QObject* parent);

      public:
         void setSocket(QTcpSocket* newSocket);

      private slots:
         void slotIncomingData();

      private:
         void sendBuffer(const Bytes& buffer) override;

      private:
         QPointer<QTcpSocket> socket;
         QByteArray buffer;
      };
   } // namespace Tunnel
} // namespace Midi

#endif // NOT MidiTunnelSocketH
