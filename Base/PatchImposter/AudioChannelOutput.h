#ifndef AudioChannelOutputH
#define AudioChannelOutputH

#include "AudioChannelAbstract.h"

namespace AudioChannel
{
   class Output : public Abstract
   {
      Q_OBJECT
   public:
      Output();

   public:
      void process(float* data, const uint64_t& blockSize);
   };
} // namespace AudioChannel

#endif // AudioChannelOutputH
