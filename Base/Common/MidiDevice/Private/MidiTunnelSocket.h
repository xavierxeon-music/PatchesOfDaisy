#ifndef MidiTunnelSocketH
#define MidiTunnelSocketH

#include <Midi/MidiInterfaceInput.h>
#include <Midi/MidiInterfaceOutput.h>

#include <QPointer>
#include <QTcpSocket>

namespace Midi
{
   namespace Tunnel
   {
      class Socket : public QObject, public Interface::Input, public Interface::Output
      {
         Q_OBJECT
      public:
         static const int port;

      public:
         Socket(QObject* parent);

      public:
         void setSocket(QTcpSocket* newSocket);
         QAbstractSocket::SocketState getState() const;

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
