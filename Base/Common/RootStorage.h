#ifndef RootStorageH
#define RootStorageH

#include "Remember.h"

class RootStorage
{
public:
   RootStorage(Remember::Root* root);

public:
   void loadFromData(const QByteArray& content);
   bool loadFromFile(const QString& fileName);

   QByteArray saveToData();
   bool saveToFile(const QString& fileName);

private:
   Remember::Root* root;
};

#endif // NOT RootStorageH
