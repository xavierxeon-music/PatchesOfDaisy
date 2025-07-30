#include "Gate.h"

#include <map>

Gate::Gate()
   : Trapezoid()
   , applyToKnobs(this, false)
{
}

bool Gate::getApplyToKnobs() const
{
   return applyToKnobs;
}

void Gate::toggleApplyToKnobs()
{
   if (applyToKnobs)
      applyToKnobs = false;
   else
      applyToKnobs = true;

   Remember::Root::setUnsynced();
}
