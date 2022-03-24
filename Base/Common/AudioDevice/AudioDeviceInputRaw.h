#ifndef AudioDeviceInputRawH
#define AudioDeviceInputRawH

#include "Private/AudioDeviceCommon.h"

namespace AudioDevice
{
   class InputRaw
   {
   public:
      InputRaw(Driver* driver, const Channel& channel);

   protected:
      virtual void process(const InputBuffer& inputBuffer);
   };
} // namespace AudioDevice

#endif // AudioDeviceInputRawH
