#ifndef AudioDeviceCommonH
#define AudioDeviceCommonH

namespace AudioDevice
{
   namespace Common // some common values
   {
      struct SampleRate
      {
         enum Value : uint32_t // do not use enum class to allow casting!
         {
            Default = 0,
            CompactDisk = 44100,
            Normal = 48000,
            High = 96000,
         };
         using BoolMap = QMap<Value, bool>;
         using StringMap = QMap<Value, QString>;

         static const StringMap nameMap;
      };

      struct Device
      {
         static inline const QString ES8 = "ES-8";
      };

   }; // namespace Common

   using Frame = unsigned long;
   using Channel = int;

   class Driver; // neede for input and output headers

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
