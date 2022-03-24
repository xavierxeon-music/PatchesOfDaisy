#ifndef ClockedTempoH
#define ClockedTempoH

#include <Music/Tempo.h>

#include <Abstract/Abstract.h>

class ClockedTempo : public Tempo
{
public:
   inline ClockedTempo();

public:
   enum class Event
   {
      None,
      Tick,
      Reset
   };

public:
   inline Event advance(const Input& input, const float audioCallackRate);

private: // block access to base class functions
   using Tempo::advance;
   using Tempo::clockReset;
   using Tempo::clockTick;

private:
   bool gateActive;
};

#include <ClockedTempo.hpp>

#endif // ClockedTempoH
