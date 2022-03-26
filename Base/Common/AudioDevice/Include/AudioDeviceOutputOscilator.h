#ifndef AudioDeviceOutputOscilatorH
#define AudioDeviceOutputOscilatorH

#include "AudioDeviceOutputRaw.h"
#include <Sound/TableOscilator.h>

#include <Sound/StandardTable.h>

namespace AudioDevice
{
   class OutputOscilator : public OutputRaw, public TableOscilator
   {
   public:
      OutputOscilator();
      OutputOscilator(Driver* driver, const Channel& channel, const Standard::Waveform::Shape& waveform);

   private:
      void process(OutputBuffer& outputBuffer) override;
   };
} // namespace AudioDevice

#endif // AudioDeviceOutputOscilatorH
