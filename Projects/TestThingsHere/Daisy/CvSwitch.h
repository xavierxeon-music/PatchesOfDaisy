#ifndef CvSwitchH
#define CvSwitchH

#include <map>

#include <MusicTools.h>

class CvSwitch
{
public:
   enum class Device
   {
      MimeticDigitalis4,
      MimeticDigitalis16,
      ViceVirga2,
      ViceVirga4,
      ViceVirga8,
      TotalRecall
   };

public:
   CvSwitch(const Device& device);

public:
   static const std::string deviceName(const Device& device);
   const Device& getDevice() const;
   const uint8_t& getMaxIndex() const;
   float map(const uint8_t index) const;

private:
   using VoltageList = std::vector<float>;

private:
   uint8_t maxIndex;
   VoltageList voltageList;
   const Device device;
};

#endif // CvSwitchH
