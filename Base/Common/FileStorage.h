#ifndef FileStorageH
#define FileStorageH

#include "Remember.h"

class FileStorage
{
public:
   FileStorage(Remember::Root* root);

public:
   void loadFromFile(const QString& fileName);
   void saveToFile(const QString& fileName);

private:
   Remember::Root* root;
};

#endif // NOT FileStorageH
