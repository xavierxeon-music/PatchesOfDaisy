#ifndef AudioDeviceOutputOscilatorH
#define AudioDeviceOutputOscilatorH

#include "AudioDeviceOutputRaw.h"
#include <Sound/WaveTable.h>

#include <Sound/StandardTable.h>

namespace AudioDevice
{
   class OutputOscilator : public OutputRaw, public WaveTable::Oscilator
   {
   public:
      OutputOscilator();
      OutputOscilator(Driver* driver, const Channel& channel, const Standard::Waveform::Shape& waveform);

   private:
      void process(OutputBuffer& outputBuffer) override;

   private:
      Standard::Table standardTable;
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputOscilatorH
