#include "Settings.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QStandardPaths>

#include "ImposterDaisyPatch.h"

Settings::Keeper Settings::keeper = {{false, {QJsonObject(), 0}}, {true, {QJsonObject(), 0}}};

Settings::Settings(const QString& groupName, bool appSettings)
   : JSONTools::AbstractSettings(groupName)
   , appSettings(appSettings)
{
   if (0 == keeper[appSettings].useCount) // read new object
   {
      QFile file(fileName());
      if (file.open(QIODevice::ReadOnly)) // file exists
      {
         QJsonParseError error;
         QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &error);
         file.close();

         keeper[appSettings].docObject = doc.object();
      }
   }
   keeper[appSettings].useCount++;
}

Settings::~Settings()
{
   if (0 == keeper[appSettings].useCount)
      Q_ASSERT(false); // this should not happen!

   keeper[appSettings].useCount--;
   if (0 < keeper[appSettings].useCount) // object still in use
      return;

   // save object
   QFile file(fileName());
   if (!file.open(QIODevice::WriteOnly))
      return;

   QJsonDocument doc(keeper[appSettings].docObject);
   file.write(doc.toJson());
   file.close();
}

QString Settings::path()
{
   static const QString path = []()
   {     
      const QString path = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation) + "/ImposterDaisyPatch";      
      QDir().mkpath(path);

      return path;
   }();

   return path;
}

QString Settings::fileName() const
{
   using NameMap = QMap<bool, QString>;
   // do not make variable a class static, because Imposter::DaisyPatch needs to be constructed first
   static const NameMap nameMap = []()
   {
      NameMap nameMap;
      const QString path = Settings::path();

      const QString appFileName = path + "/Settings.json";
      qDebug() << "app settings file @ " << appFileName;
      nameMap[true] = appFileName;

      const QString fileName = path + "/Settings_" + Imposter::DaisyPatch::getAppName() + ".json";
      qDebug() << "settings file @ " << fileName;
      nameMap[false] = fileName;

      return nameMap;
   }();

   return nameMap[appSettings];
}

QJsonObject& Settings::contentRef()
{
   return keeper[appSettings].docObject;
}

const QJsonObject& Settings::content() const
{
   return keeper[appSettings].docObject;
}
