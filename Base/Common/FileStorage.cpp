#include "FileStorage.h"

#include <cstring>

#include <QFile>

FileStorage::FileStorage(Remember::Root* root)
   : root(root)
{
}

bool FileStorage::loadFromFile(const QString& fileName)
{
   if (!root)
      return false;

   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
      return false;

   const QByteArray content = file.readAll();
   file.close();

   const uint64_t dataSize = content.size();

   Remember::DataVector data(dataSize);
   std::memcpy(&data[0], content.constData(), dataSize);
   root->set(data);

   return true;
}

bool FileStorage::saveToFile(const QString& fileName)
{
   if (!root)
      return false;

   Remember::DataVector data = root->get();
   const uint64_t dataSize = data.size();

   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return false;

   file.write((const char*)(&data[0]), dataSize);
   file.close();
   root->setSynced();

   return true;

}
