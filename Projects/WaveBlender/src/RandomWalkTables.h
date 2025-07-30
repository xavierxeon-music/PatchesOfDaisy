#ifndef RandomWalkTablesH
#define RandomWalkTablesH

#include <Sound/WaveTable.h>

#include <Global.h>

class RandomWalkTables : public WaveTable::StepTable
{
public:
   static const uint8_t maxSeed;

public:
   RandomWalkTables();

public:
   void setSeed(const uint8_t& newSeed);
   float valueByAngle(const float& angle) const;

private:
   struct Index
   {
      uint8_t major; // 0 to 64
      uint8_t minor; // 0 to 64
   };

private:
   float valueFromIndex(const uint8_t seed, const Index& index) const;

private:
   uint8_t seed;
   float tables[128][64]; // seed, index
};

#endif // RandomWalkTablesH
