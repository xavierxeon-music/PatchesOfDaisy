#ifndef KeyH
#define KeyH

#include <map>
#include <string>

struct Key
{
   const std::string name;
   const bool black;

   using Map = std::map<uint8_t, Key>;
   static const Key::Map map;
};

#endif // KeyH
