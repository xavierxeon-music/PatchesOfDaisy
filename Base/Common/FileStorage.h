#ifndef FileStorageH
#define FileStorageH

#include "Remember.h"

class FileStorage
{
public:
   FileStorage(Remember::Root* root);

public:
   bool loadFromFile(const QString& fileName);
   bool saveToFile(const QString& fileName);

private:
   Remember::Root* root;
};

#endif // NOT FileStorageH
