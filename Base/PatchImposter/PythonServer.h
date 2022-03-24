#ifndef PythonServerH
#define PythonServerH

#include <QLocalServer>

#include <functional>

#include <QJsonObject>
#include <QLocalSocket>
#include <QPointer>

namespace Python
{
   class Server : public QLocalServer
   {
      Q_OBJECT
   public:
      using ClientCallFunction = std::function<QJsonObject(const QJsonObject&)>;

   public:
      Server(QObject* parent);
      virtual ~Server();

   public:
      // send to client and get response
      QJsonObject sendToClient(const QJsonObject& localObject);
      // client calls server and expects response
      void setClientCallFunction(ClientCallFunction newClientCallFunction);

   private slots:
      void slotNewConnection();
      void slotRemoteRead();

   private:
      QJsonObject dummyClientCall(const QJsonObject& remoteObject);

   private:
      QPointer<QLocalSocket> remote;
      QString serverFile;
      ClientCallFunction clientCallFunction;
   };
} // namespace Python

#endif // PythonServerH
