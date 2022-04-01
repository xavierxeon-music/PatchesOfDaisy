#include "Private/MidiTunnelSocket.h"

const int Midi::Tunnel::Socket::port = 6666;

Midi::Tunnel::Socket::Socket(QObject* parent)
   : QObject(parent)
   , Interface()
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
   qDebug() << __FUNCTION__;

   buffer.append(socket->readAll());
   while (buffer.length() >= 3)
   {
      const QByteArray messageBuffer = buffer.right(3);
      buffer.chop(3);

      Bytes message(3);
      std::memcpy(&message[0], (uint8_t*)messageBuffer.data(), 3);

      const Midi::Channel channel = message[0] & 0x0F;
      if (Midi::Event::NoteOn == (message[0] & 0xF0))
      {
         const Note note = Note::fromMidi(message[1]);
         const Midi::Velocity velocity = message[2];

         for (const NoteOnFunction& noteOnFunction : noteOnFunctionList)
            noteOnFunction(channel, note, velocity);
      }
      else if (Midi::Event::NoteOff == (message[0] & 0xF0))
      {
         const Note note = Note::fromMidi(message[1]);

         for (const NoteOffFunction& noteOffFunction : noteOffFunctionList)
            noteOffFunction(channel, note);
      }
      else if (Midi::Event::ControlChange == (message[0] & 0xF0))
      {
         const Midi::ControllerMessage controllerMessage = static_cast<Midi::ControllerMessage>(message[1]);
         const uint8_t value = message[2];

         for (const ControllChangeFunction& controllChangeFunction : controllChangeFunctionList)
            controllChangeFunction(channel, controllerMessage, value);
      }
   }
}

void Midi::Tunnel::Socket::sendBuffer(const Bytes& buffer)
{
   if (socket.isNull())
      return;

   const char* data = (const char*)buffer.data();
   socket->write(data, buffer.size());
}
