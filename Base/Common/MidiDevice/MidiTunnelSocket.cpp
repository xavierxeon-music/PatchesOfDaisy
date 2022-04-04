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
   QByteArray buffer = socket->readAll();
   static const uint8_t mask = 0x80;

   qsizetype startIndex = 0;
   for (qsizetype index = 0; index < buffer.size(); index++)
   {
      const uint8_t byte = buffer.at(index);

      if (index + 1 == buffer.size())
         index += 1;

      const bool messageStart = (mask == (byte & mask));
      if ((messageStart && index != startIndex) // mew message, but not first entry
          || (index == buffer.size()))          // end of buffer
      {
         const qsizetype length = index - startIndex;
         Bytes message(length);
         std::memcpy(&message[0], (uint8_t*)buffer.mid(startIndex, length).data(), length);

         for (Interface::Output* passthrough : passthroughList)
            passthrough->sendBuffer(message);

         dataFromInput(message);

         startIndex = index;
      }
   }
}

void Midi::Tunnel::Socket::sendBuffer(const Bytes& message)
{
   if (socket.isNull() || !socket->isOpen())
      return;

   const char* data = (const char*)message.data();
   socket->write(data, message.size());

   socket->flush();
   QThread::msleep(1);
}
