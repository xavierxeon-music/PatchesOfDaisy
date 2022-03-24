#ifndef ClockedTempoHPP
#define ClockedTempoHPP

#include <Music/ClockedTempo.h>

ClockedTempo::ClockedTempo()
   : Tempo()
   , gateActive(false)
{
}

ClockedTempo::Event ClockedTempo::advance(const Input& input, const float audioCallackRate)
{
   if (!input.gates[0])
      gateActive = false;

   if (input.gates[1])
   {
      clockReset();
      return Event::Reset;
   }
   else if (input.gates[0] && !gateActive)
   {
      gateActive = true;
      clockTick();
      return Event::Tick;
   }

   Tempo::advance(audioCallackRate);

   return Event::None;
}

#endif // ClockedTempoHPP
