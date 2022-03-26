#ifndef AudioDeviceInputCVH
#define AudioDeviceInputCVH

#include "AudioDeviceInputRaw.h"

namespace AudioDevice
{
   class InputCV : public InputRaw
   {
   public:
      InputCV();
      InputCV(Driver* driver, const Channel& channel);

   public:
      // valid voltages range from -10V to +10V
      const float& getVoltage() const;

   private:
      void process(const InputBuffer& inputBuffer) override;

   private:
      float voltage;
   };
} // namespace AudioDevice

#endif // AudioDeviceInputCVH
