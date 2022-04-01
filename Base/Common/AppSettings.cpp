#include "AppSettings.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QStandardPaths>

QJsonObject AppSettings::docObject = QJsonObject();
quint32 AppSettings::useCount = 0;

AppSettings::AppSettings(const QString& groupName)
   : JSONTools::AbstractSettings(groupName)
{
   if (0 == useCount) // read new object
   {
      QFile file(fileName());
      if (file.open(QIODevice::ReadOnly)) // file exists
      {
         QJsonParseError error;
         QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
         file.close();

         docObject = doc.object();
      }
   }
   useCount++;
}

AppSettings::~AppSettings()
{
   if (0 == useCount)
      Q_ASSERT(false); // this should not happen!

   useCount--;
   if (0 < useCount) // object still in use
      return;

   // save object
   QFile file(fileName());
   if (!file.open(QIODevice::WriteOnly))
      return;

   QJsonDocument doc(docObject);
   file.write(doc.toJson());
   file.close();
}

const QString& AppSettings::fileName()
{
   // do not make variable a class static, because QApplication needs to be constructed first!
   static const QString settingsFileName = [&]()
   {
      const QString appName = QCoreApplication::applicationName();
      const QString domainName = QCoreApplication::organizationDomain();
      if (appName.isEmpty() || domainName.isEmpty())
         qFatal("Setings: must set app and domain first");

      const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/" + domainName + "." + appName;
      QDir().mkpath(path);
      QString fileName = path + "/settings.json";
      qDebug() << "settings file @ " << fileName;
      return fileName;
   }();

   return settingsFileName;
}

QJsonObject& AppSettings::contentRef()
{
   return docObject;
}

const QJsonObject& AppSettings::content() const
{
   return docObject;
}

