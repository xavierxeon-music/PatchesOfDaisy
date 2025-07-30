#include "CvSwitchCalibration.h"

#include <Tools/SquareSignal.h>
#include <Tools/Variable.h>

#include "ThePage.h"

CvSwitchCalibration::CvSwitchCalibration()
   : Abstract::Patch("CvSwitchCalibration")
   , device(CvSwitch::StandardDevices::TargetDevice::TotalRecall)
   , cvSwitch(CvSwitch::StandardDevices::channelCount(device), CvSwitch::StandardDevices::offsetMap(device))
   , index(0)
   , voltage(0.0)
   , offsetBuffer()
   , offset(0.0)
   , manualOffset(true)
{
   pageManager.addPage(new ThePage(this));
}

std::string CvSwitchCalibration::getDeviceName() const
{
   return CvSwitch::StandardDevices::name(device);
}

void CvSwitchCalibration::changeDevice()
{
   static const std::vector<CvSwitch::StandardDevices::TargetDevice> order = {
      CvSwitch::StandardDevices::TargetDevice::MimeticDigitalis4,
      CvSwitch::StandardDevices::TargetDevice::MimeticDigitalis16,
      CvSwitch::StandardDevices::TargetDevice::ViceVirga2,
      CvSwitch::StandardDevices::TargetDevice::ViceVirga4,
      CvSwitch::StandardDevices::TargetDevice::ViceVirga8,
      CvSwitch::StandardDevices::TargetDevice::TotalRecall

   };
   Variable::Enum<CvSwitch::StandardDevices::TargetDevice> var(device, order, true);
   var.change(true);

   cvSwitch = CvSwitch::StandardDevices::create(device);
   index = 0;
}

const uint8_t& CvSwitchCalibration::getIndex() const
{
   return index;
}

void CvSwitchCalibration::changeIndex(bool up)
{
   Variable::Integer<uint8_t> var(index, 0, cvSwitch.getMaxIndex() - 1, true);
   var.change(up);
}

const float& CvSwitchCalibration::getVoltage() const
{
   return voltage;
}

const float& CvSwitchCalibration::getOffset() const
{
   return offset;
}

const bool& CvSwitchCalibration::getManualOffset() const
{
   return manualOffset;
}

void CvSwitchCalibration::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   // pass on audio
   for (uint8_t channel = 0; channel < 4; channel++)
   {
      for (size_t chunk = 0; chunk < size; chunk++)
      {
         out[channel][chunk] = in[channel][chunk];
      }
   }

   const Input input = compileInput();
   manualOffset = (input.controlVoltages[3] > 1.0);

   const float knobValue = 0.1 * (input.controlVoltages[0] - 2.5);
   offsetBuffer += knobValue;
   offset = offsetBuffer.average();

   Output output;
   if (manualOffset)
   {
      voltage = cvSwitch.map(index, false);
      output.controlVoltages[0] = offset + voltage;
   }
   else
   {
      voltage = cvSwitch.map(index, true);
      output.controlVoltages[0] = voltage;
   }

   applyOutput(output);

   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);
}

void CvSwitchCalibration::nonAudioLoop()
{
   // pretend to be alive
   static SquareSignal square(10);
   daisy.seed.SetLed(square.valueAndNext());
}

// main function

int main(void)
{
   CvSwitchCalibration app;
   app.run();
}
