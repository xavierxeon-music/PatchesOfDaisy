#include "Private/MidiTunnelSocket.h"

#include <cstring>

#include <QThread>

#include <QCoreApplication>

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
   buffer.clear();
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
   while (buffer.size() > 0)
   {
      const uint8_t messageSize = static_cast<const uint8_t>(buffer.at(0));
      if (buffer.size() < messageSize + 1)
         break;

      QByteArray messageData = buffer.mid(1, messageSize);
      buffer.remove(0, messageSize + 1);

      Bytes message(messageSize);
      std::memcpy(&message[0], messageData.constData(), messageSize);

      dataFromInput(message);
   }
}

void Midi::Tunnel::Socket::sendBuffer(const Bytes& message)
{
   if (socket.isNull() || !socket->isOpen())
      return;

   QByteArray data((const char*)message.data(), message.size());
   const uint8_t messageSize = message.size();
   data.prepend(messageSize);

   socket->write(data);

   socket->flush();
   QThread::msleep(1);
}
