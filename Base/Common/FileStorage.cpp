#include "FileStorage.h"

#include <QFile>

FileStorage::FileStorage(Remember::Root* root)
   : root(root)
{
}

void FileStorage::loadFromFile(const QString& fileName)
{
   if (!root)
      return;

   QFile file(fileName);
   if (!file.open(QIODevice::ReadOnly))
      return;

   const QByteArray content = file.readAll();
   file.close();

   const uint64_t dataSize = content.size();

   Remember::DataVector data(dataSize);
   std::memcpy(&data[0], content.constData(), dataSize);
   root->set(data);
}

void FileStorage::saveToFile(const QString& fileName)
{
   if (!root)
      return;

   Remember::DataVector data = root->get();
   const uint64_t dataSize = data.size();

   QFile file(fileName);
   if (file.open(QIODevice::WriteOnly))
   {
      file.write((const char*)(&data[0]), dataSize);
      file.close();
   }
}
