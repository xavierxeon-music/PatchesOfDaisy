#include "CvSwitch.h"

/*
#include <QDebug>

static int printVoltageMaps = []()
{
   for (const uint8_t maxValue : {2, 4, 8, 16})
   {
      const float diff = 1.0 / maxValue;
      const float offset = 0.5 * diff;
      //qDebug() << diff << offset;
      {
         auto stream = qDebug();
         stream << "voltageMap[" << maxValue << "]={";

         for (uint8_t index = 0; index < maxValue; index++)
         {
            const float value = offset + (index * diff);
            if (0 != index)
               stream << ", ";
            stream << value;
         }

         stream << "};";
      }
   }

   return 0;
}();
//*/

CvSwitch::CvSwitch(const Device& device)
   : maxIndex(0)
   , voltageList()
   , device(device)
{
   /*
   voltageMap[2] = {0.0, 1.0};
   voltageMap[4] = {0.0, 0.333333, 0.666667, 1.0};
   voltageMap[8] = {0.0, 0.142857, 0.285714, 0.428571, 0.571429, 0.714286, 0.857143, 1.0};
   voltageMap[16] = {0.0, 0.0666667, 0.133333, 0.2, 0.266667, 0.333333, 0.4, 0.466667, 0.533333, 0.6, 0.666667, 0.733333, 0.8, 0.866667, 0.933333, 1.0};
   */

   switch (device)
   {
      case Device::MimeticDigitalis4:
         maxIndex = 4;
         voltageList = {0.125, 0.375, 0.625, 0.875};
         break;
      case Device::MimeticDigitalis16:
         maxIndex = 16;
         voltageList = {0.03125, 0.09375, 0.15625, 0.21875, 0.28125, 0.34375, 0.40625, 0.46875, 0.53125, 0.59375, 0.65625, 0.71875, 0.78125, 0.84375, 0.90625, 0.96875};
         break;
      case Device::ViceVirga2:
         maxIndex = 2;
         voltageList = {0.25, 0.75};
         break;
      case Device::ViceVirga4:
         maxIndex = 4;
         voltageList = {0.125, 0.375, 0.625, 0.875};
         break;
      case Device::ViceVirga8:
         maxIndex = 8;
         voltageList = {0.0625, 0.1875, 0.3125, 0.4375, 0.5625, 0.6875, 0.8125, 0.9375};
         break;
      case Device::TotalRecall:
         maxIndex = 8;
         voltageList = {0.0625, 0.1875, 0.3125, 0.4375, 0.5625, 0.6875, 0.8125, 0.9375};
         break;
      default:
         break;
   }
}

const std::string CvSwitch::deviceName(const Device& device)
{
   std::string name = "Unknown";
   switch (device)
   {
      case Device::MimeticDigitalis4:
         name = "Mimetic Digitalis 4";
         break;
      case Device::MimeticDigitalis16:
         name = "Mimetic Digitalis 16";
         break;
      case Device::ViceVirga2:
         name = "Vice Virga 2";
         break;
      case Device::ViceVirga4:
         name = "Vice Virga 4";
         break;
      case Device::ViceVirga8:
         name = "Vice Virga 8";
         break;
      case Device::TotalRecall:
         name = "Total Recall";
         break;
      default:
         break;
   }

   return name;
}

const CvSwitch::Device& CvSwitch::getDevice() const
{
   return device;
}

const uint8_t& CvSwitch::getMaxIndex() const
{
   return maxIndex;
}

float CvSwitch::map(const uint8_t index) const
{
   if (index >= maxIndex)
      return 0.0;

   const float& value = voltageList.at(index);
   return value;
}
