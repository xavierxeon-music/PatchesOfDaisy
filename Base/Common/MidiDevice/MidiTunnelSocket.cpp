#include "Private/MidiTunnelSocket.h"

#include <cstring>

const int Midi::Tunnel::Socket::port = 6666;

Midi::Tunnel::Socket::Socket(QObject* parent)
   : QObject(parent)
   , Interface::Input()
   , Interface::Output()
   , socket()
   , buffer()
{
}

void Midi::Tunnel::Socket::setSocket(QTcpSocket* newSocket)
{
   if (!socket.isNull())
   {
      disconnect(socket, &QTcpSocket::readyRead, this, &Socket::slotIncomingData);
      socket->deleteLater();
   }

   socket = newSocket;
   if (socket)
   {
      connect(socket, &QTcpSocket::readyRead, this, &Socket::slotIncomingData);
   }
}

QAbstractSocket::SocketState Midi::Tunnel::Socket::getState() const
{
   if (socket.isNull())
      return QAbstractSocket::UnconnectedState;

   return socket->state();
}

void Midi::Tunnel::Socket::slotIncomingData()
{
   buffer.append(socket->readAll());
   while (buffer.length() >= 3)
   {
      const QByteArray messageBuffer = buffer.left(3);
      buffer.remove(0, 3);

      Bytes message(3);
      std::memcpy(&message[0], (uint8_t*)messageBuffer.data(), 3);

      for (Interface::Output* passthrough : passthroughList)
         passthrough->sendBuffer(message);

      dataFromInput(message);
   }
}

void Midi::Tunnel::Socket::sendBuffer(const Bytes& buffer)
{
   if (socket.isNull())
      return;

   const char* data = (const char*)buffer.data();
   socket->write(data, buffer.size());

   socket->flush();
}
