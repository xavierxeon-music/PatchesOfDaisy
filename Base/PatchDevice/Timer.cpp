#include <Tools/Timer.h>

#include <limits>

#ifndef NON_DAISY_DEVICE

#include "daisy_seed.h"
using System = daisy::System;

#else

#include <QElapsedTimer>
static QElapsedTimer* internalTimer = []
{
   QElapsedTimer* timer = new QElapsedTimer;
   return timer;
}();

#endif // NON_DAISY_DEVICE

const uint32_t Timer::uSecsPerSecond = 1000 * 1000;
const uint32_t Timer::uSecsPerMinute = 60 * uSecsPerSecond;
const uint32_t Timer::invalid = std::numeric_limits<uint32_t>::max() - 1;

Timer::Timer()
   : startTime(0)
{
   start();
}

void Timer::start()
{
#ifndef NON_DAISY_DEVICE
   startTime = System::GetUs();
#else
   internalTimer->start();
   startTime = 0;
#endif // NON_DAISY_DEVICE
}

uint32_t Timer::elapsed() const
{
#ifndef NON_DAISY_DEVICE
   const uint32_t now = System::GetUs();
#else
   const uint32_t now = internalTimer->elapsed() * 1000.0;
#endif // NON_DAISY_DEVICE

   if (now < startTime) // more than an hour has passsed since start was called
      return invalid;

   const uint32_t diff = now - startTime;
   return diff;
}
