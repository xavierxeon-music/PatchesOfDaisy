#include "PythonServer.h"

#include <QApplication>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

Python::Server::Server(QObject* parent)
   : QLocalServer(parent)
   , remote()
   , serverFile()
   , clientCallFunction()
{
   ClientCallFunction defaultClientCall = std::bind(&Server::dummyClientCall, this, std::placeholders::_1);
   setClientCallFunction(defaultClientCall);

   serverFile = QStandardPaths::writableLocation(QStandardPaths::TempLocation) + "/ImposterDaisyPatch.server";

   auto otherServerRunning = [&]()
   {
      if (!QFile::exists(serverFile))
         return false; // no file means no server

      // file exists, therfore either server is running ...
      QLocalSocket socket;
      socket.connectToServer(serverFile);
      if (socket.waitForConnected(1000))
         return true;

      // ... or previous cleanup failed
      if (!QFile::remove(serverFile))
         return true; // something is blocking the file

      return false;
   };

   if (otherServerRunning())
      QApplication::exit(1);

   qDebug() << "client server running @" << serverFile;

   connect(this, &QLocalServer::newConnection, this, &Server::slotNewConnection);
   listen(serverFile);
}

Python::Server::~Server()
{
   close();
   QFile::remove(serverFile);

   qDebug() << "closed client server";
}

QJsonObject Python::Server::sendToClient(const QJsonObject& localObject)
{
   if (remote.isNull())
   {
      QJsonObject errorObject;
      errorObject["reply"] = "NoRemote";
      return errorObject;
   }
   else if (remote->state() != QLocalSocket::ConnectedState)
   {
      remote->deleteLater();

      QJsonObject errorObject;
      errorObject["reply"] = "NotConnected";
      return errorObject;
   }

   // send
   const QByteArray request = QJsonDocument(localObject).toJson();
   remote->write(request);
   remote->waitForBytesWritten();

   //receive
   const QByteArray reply = remote->readAll();
   remote->waitForBytesWritten();

   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(reply, &error);
   QJsonObject remoteObject = doc.object();

   return remoteObject;
}

void Python::Server::setClientCallFunction(ClientCallFunction newClientCallFunction)
{
   clientCallFunction = newClientCallFunction;
}

void Python::Server::slotNewConnection()
{
   remote = nextPendingConnection();
   //connect(remote, &QLocalSocket::readyRead, this, &Server::slotRemoteRead);
}

void Python::Server::slotRemoteRead()
{
   if (remote.isNull() || !remote->isOpen())
      return;

   // receive
   const QByteArray request = remote->readAll();

   QJsonParseError error;
   QJsonDocument doc = QJsonDocument::fromJson(request, &error);
   QJsonObject remoteObject = doc.object();

   // process
   QJsonObject localObject = clientCallFunction(remoteObject);

   // send
   const QByteArray reply = QJsonDocument(localObject).toJson();
   remote->write(reply);
   remote->waitForBytesWritten();
}

QJsonObject Python::Server::dummyClientCall(const QJsonObject& remoteObject)
{
   Q_UNUSED(remoteObject);

   QJsonObject nullObject;
   nullObject["reply"] = "NotImplementd";
   return nullObject;
}
