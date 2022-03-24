#ifndef AudioDeviceOutputRawH
#define AudioDeviceOutputRawH

#include "../Private/AudioDeviceCommon.h"

namespace AudioDevice
{
   class OutputRaw
   {
   public:
      OutputRaw(Driver* driver, const Channel& channel);

   protected:
      virtual void process(OutputBuffer& outputBuffer);
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputRawH
