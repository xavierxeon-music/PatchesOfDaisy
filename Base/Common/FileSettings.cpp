#include "FileSettings.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>

QString FileSettings::fileName = QString();
QJsonObject FileSettings::docObject = QJsonObject();
quint32 FileSettings::useCount = 0;

FileSettings::FileSettings(const QString& groupName)
   : JSONTools::AbstractSettings(groupName)
{
   if (fileName.isEmpty())
      return;

   if (0 == useCount) // read new object
   {
      QFile file(fileName);
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

FileSettings::~FileSettings()
{
   if (fileName.isEmpty())
      return;

   if (0 == useCount)
      Q_ASSERT(false); // this should not happen!

   useCount--;
   if (0 < useCount) // object still in use
      return;

   // save object
   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return;

   QJsonDocument doc(docObject);
   file.write(doc.toJson());
   file.close();
}

const QString& FileSettings::getFileName()
{
   return fileName;
}

void FileSettings::setFileName(const QString& newFileName)
{
   fileName = newFileName;

   useCount = 0;
   docObject = QJsonObject();

   QFile file(fileName);
   if (!file.exists()) // write empty file
   {
      if (!file.open(QIODevice::WriteOnly))
         return;

      QJsonDocument doc(docObject);
      file.write(doc.toJson());
      file.close();
   }
}

QJsonObject& FileSettings::contentRef()
{
   return docObject;
}

const QJsonObject& FileSettings::content() const
{
   return docObject;
}
