#ifndef SDCardHPP
#define SDCardHPP

#include <Storage/SDCard.h>

template <typename DataType>
void SDCard::load(DataType* data)
{
   load((char*)data, sizeof(DataType));
}

template <typename DataType>
void SDCard::save(const DataType* data)
{
   save((const char*)data, sizeof(DataType));
}

#endif // SDCardHPP
