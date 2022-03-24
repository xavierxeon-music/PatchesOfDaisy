#ifndef AudioDeviceCommonH
#define AudioDeviceCommonH

namespace AudioDevice
{
   using Frame = unsigned long;
   using Channel = int;

   class Driver;

   struct InputBuffer
   {
      virtual const float& operator[](const Frame& frame) const = 0;
      virtual const Frame& getNoOfFrames() const = 0;
   };

   struct OutputBuffer
   {
      virtual float& operator[](const Frame& frame) = 0;
      virtual const Frame& getNoOfFrames() const = 0;
   };
} // namespace AudioDevice

#endif // AudioDeviceCommonH
