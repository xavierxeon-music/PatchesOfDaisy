#ifndef AudioDeviceInputRawH
#define AudioDeviceInputRawH

#include "../Private/AudioDeviceCommon.h"

namespace AudioDevice
{
   class InputRaw
   {
   public:
      InputRaw();
      InputRaw(Driver* driver, const Channel& channel); // calls activate

   public:
      virtual void activate(Driver* driver, const Channel& channel);

   protected:
      virtual void process(const InputBuffer& inputBuffer);
   };
} // namespace AudioDevice

#endif // AudioDeviceInputRawH
