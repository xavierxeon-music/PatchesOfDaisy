#ifndef TimerH
#define TimerH

#include <Global.h>

// times are in uSecs,
// uint32_t should hold around 4294 seconds, slightly more than an hour
// but hardware seems to return "invalid" approx every 20 seconds

class Timer
{
public:
   Timer();

public:
   void start();
   uint32_t elapsed() const;

public:
   static const uint32_t uSecsPerSecond;
   static const uint32_t uSecsPerMinute; // used by Tempo for BPM count
   static const uint32_t invalid;

private:
   uint32_t startTime;
};

#endif // TimerH
