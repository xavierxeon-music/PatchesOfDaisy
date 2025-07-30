#include "RandomWalkTables.h"

#include <limits>
#include <map>

#include <Tools/FastRandom.h>
#include <Tools/Range.h>

const uint8_t RandomWalkTables::maxSeed = 127;

RandomWalkTables::RandomWalkTables()
   : WaveTable::StepTable(4096, 4.0f * Maths::pi)
   , seed(0)
{
   for (uint8_t tmpSeed = 0; tmpSeed <= maxSeed; tmpSeed++)
   {
      FastRandom random(tmpSeed);
      float value = 0.0f;
      const float size = 0.3;
      for (uint8_t index = 0; index < 64; index++)
      {
         value += (2.0f * size * random.value()) - size;
         value = Range::clamp<float>(value, -1.0, 1.0);

         tables[tmpSeed][index] = value;
      }
   }
}

void RandomWalkTables::setSeed(const uint8_t& newSeed)
{
   seed = newSeed;
}

float RandomWalkTables::valueByAngle(const float& angle) const
{
   const uint64_t fullIndex = stepIndexFromAngle(angle);

   Index index;
   index.minor = fullIndex % 64;
   index.major = (fullIndex - index.minor) / 64;

   return valueFromIndex(seed, index);
}

float RandomWalkTables::valueFromIndex(const uint8_t seed, const Index& index) const
{
   const float majorValue = tables[seed][index.major];
   if (63 == index.major)
      return majorValue;

   const float percentage = static_cast<float>(index.minor) / 64.0;
   const float nextValue = tables[seed][index.major + 1];
   const float diff = percentage * (nextValue - majorValue);

   const float value = majorValue + diff;
   return value;
}
