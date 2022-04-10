#include "RootStorage.h"

#include <cstring>

#include <QFile>

RootStorage::RootStorage(Remember::Root* root)
   : root(root)
{
}

void RootStorage::loadFromData(const QByteArray& content)
{
   if (!root || content.isEmpty())
      return;

   const uint64_t dataSize = content.size();

   Remember::DataVector data(dataSize);
   std::memcpy(&data[0], content.constData(), dataSize);
   root->set(data);
}

bool RootStorage::loadFromFile(const QString& fileName)
{
   if (!root)
      return false;

   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
      return false;

   const QByteArray content = file.readAll();
   file.close();

   loadFromData(content);
   return true;
}

QByteArray RootStorage::saveToData()
{
   if (!root)
      return QByteArray();

   Remember::DataVector data = root->get();
   const uint64_t dataSize = data.size();

   QByteArray content;
   content.resize(dataSize);

   std::memcpy(content.data(), data.data(), dataSize);

   return content;
}

bool RootStorage::saveToFile(const QString& fileName)
{
   if (!root)
      return false;

   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return false;

   const QByteArray content = saveToData();

   file.write(content);
   file.close();

   root->setSynced();

   return true;

}
