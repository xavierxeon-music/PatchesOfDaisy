#include "MidiTunnelServer.h"

Midi::Tunnel::Server::Server(QObject* parent)
   : Socket(parent)
   , server(nullptr)
{
   server = new QTcpServer(this);

   connect(server, &QTcpServer::newConnection, this, &Server::slotHasNewConnection);
   server->listen(QHostAddress::Any, port);
}

void Midi::Tunnel::Server::slotHasNewConnection()
{
   QTcpSocket* newSocket = server->nextPendingConnection();
   setSocket(newSocket);
   qInfo() << "new connection";
}
