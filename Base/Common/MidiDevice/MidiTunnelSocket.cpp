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
   qDebug() << __FUNCTION__ << buffer.size();
   while (buffer.length() >= 3)
   {
      const QByteArray messageBuffer = buffer.left(3);
      buffer.remove(0, 3);

      Bytes message(3);
      std::memcpy(&message[0], (uint8_t*)messageBuffer.data(), 3);

      for (Interface::Output* passthrough : passthroughList)
         passthrough->sendBuffer(message);

      const Midi::Channel channel = message[0] & 0x0F;
      const Midi::Event event = static_cast<Midi::Event>(message[0] & 0xF0);

      if (Midi::Event::NoteOn == event)
      {
         const Note note = Note::fromMidi(message[1]);
         const Midi::Velocity velocity = message[2];

         qDebug() << "note on" << channel << note.midiValue << velocity;

         for (const NoteOnFunction& noteOnFunction : noteOnFunctionList)
            noteOnFunction(channel, note, velocity);
      }
      else if (Midi::Event::NoteOff == event)
      {
         const Note note = Note::fromMidi(message[1]);

         qDebug() << "note off" << channel << note.midiValue;

         for (const NoteOffFunction& noteOffFunction : noteOffFunctionList)
            noteOffFunction(channel, note);
      }
      else if (Midi::Event::ControlChange == event)
      {
         const Midi::ControllerMessage controllerMessage = static_cast<Midi::ControllerMessage>(message[1]);
         const uint8_t value = message[2];

         qDebug() << "controller change" << channel << controllerMessage << value;

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

   socket->flush();
}
