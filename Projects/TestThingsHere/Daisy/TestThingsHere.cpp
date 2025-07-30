#include "TestThingsHere.h"

#include <Tools/SquareSignal.h>
#include <Tools/Variable.h>

#include "ThePage.h"

TestThingsHere::TestThingsHere()
   : Abstract::Patch("TestThingsHere")
   , cvSwitch(CvSwitch::Device::MimeticDigitalis16)
   , index(0)
   , voltage(0.0)
   , offsetBuffer()
   , offset(0.0)
{
   pageManager.addPage(new ThePage(this));
}

std::string TestThingsHere::getDeviceName() const
{
   const CvSwitch::Device& device = cvSwitch.getDevice();
   return CvSwitch::deviceName(device);
}

void TestThingsHere::changeDevice()
{
   /*
   static const std::vector<uint8_t> maxValues = {1, 3, 7, 15};
   Variable::Enum<uint8_t> var(maxValue, maxValues, true);
   var.change(true);

   using CvMaxValueMap = std::map<uint8_t, CvSwitch::Type>;
   static const CvMaxValueMap cvMaxValueMap = {{1, CvSwitch::Type::Two}, {3, CvSwitch::Type::Four}, {7, CvSwitch::Type::Eight}, {15, CvSwitch::Type::Sixteen}};
   cvSwitch.setType(cvMaxValueMap.at(maxValue));
   */
   index = 0;
}

const uint8_t& TestThingsHere::getIndex() const
{
   return index;
}

void TestThingsHere::changeIndex(bool up)
{
   Variable::Integer<uint8_t> var(index, 0, cvSwitch.getMaxIndex() - 1, true);
   var.change(up);
}

const float& TestThingsHere::getVoltage() const
{
   return voltage;
}

const float& TestThingsHere::getOffset() const
{
   return offset;
}

void TestThingsHere::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
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

   voltage = cvSwitch.map(index);

   const float knobValue = 1.0 * (input.knobs[0] - 0.5);
   offsetBuffer += knobValue;
   offset = offsetBuffer.average();
   qDebug() << knobValue << offset;

   Output output;
   output.cvs[0] = voltage;
   applyOutput(output);

   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);
}

void TestThingsHere::nonAudioLoop()
{
   // pretend to be alive
   static SquareSignal square(10);
   daisy.seed.SetLed(square.valueAndNext());
}

// main function

int main(void)
{
   TestThingsHere app;
   app.run();
}
