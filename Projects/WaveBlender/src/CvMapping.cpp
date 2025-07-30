#include "CvMapping.h"

#include <Tools/Range.h>

#include "WaveBlender.h"

CvMapping::CvMapping()
   : Remember::Container()
   , sums()
   , bitFieldStore(this, 0)
{
}

void CvMapping::apply(const float controlVoltages[4])
{
   for (uint8_t type = 0; type < 4; type++)
   {
      Sum& sum = sums[type];
      sum.value = 0.0;
      sum.active = false;

      for (uint8_t channel = 0; channel < 4; channel++)
      {
         if (!get(static_cast<Type>(type), channel))
            continue;

         sum.value += controlVoltages[channel];
         sum.active = true;
      }

      sum.value = Range::clamp<float>(sum.value, 0.0, 5.0);
   }
}

CvMapping::Sum CvMapping::sum(const Type& type)
{
   return sums[type];
}

void CvMapping::set(const Type& type, const uint8_t& channel, bool on)
{
   BoolField16 bitField(bitFieldStore);
   const uint8_t index = (4 * type) + channel;

   bitField.set(index, on);
   bitFieldStore = bitField;
   Remember::Root::setUnsynced();
}

bool CvMapping::get(const Type& type, const uint8_t& channel) const
{
   BoolField16 bitField(bitFieldStore);
   const uint8_t index = (4 * type) + channel;

   return bitField.get(index);
}
