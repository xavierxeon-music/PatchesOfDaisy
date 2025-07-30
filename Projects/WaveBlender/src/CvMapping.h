#ifndef CvMappingH
#define CvMappingH

#include <Remember.h>

#include <Tools/BoolField.h>

class CvMapping : public Remember::Container
{
public:
   struct Sum
   {
      float value = 0.0;
      bool active = false;
   };

   enum Type
   {
      Pitch = 0,
      Seed,
      Blend,
      Other
   };

public:
   CvMapping();

public:
   void apply(const float controlVoltages[4]);
   Sum sum(const Type& type);

   void set(const Type& type, const uint8_t& channel, bool on);
   bool get(const Type& type, const uint8_t& channel) const;

private: // things to remember
   using BitFieldStore_ = Remember::Value<uint16_t>;

private:
   Sum sums[4];
   BitFieldStore_ bitFieldStore;
};

#endif //  CvMappingH
