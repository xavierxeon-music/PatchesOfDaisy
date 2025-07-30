#include "Rhythm.h"

#include <Tools/Variable.h>

Rhythm::Rhythm()
   : Remember::Container()
   , velocities(this)
   , numberOfSteps(this, 0)
   , stepSize(this, Tempo::Division::Eigth)
   , stepSizeCounter(0)
   , indexCounter(maxLength)
   , firstTickDone(false)
   , lastIndex(0)
{
   for (uint8_t index = 0; index < maxLength; index++)
      velocities[index] = 0;
}

void Rhythm::init()
{
   stepSizeCounter.setMaxValue(static_cast<uint8_t>(stepSize));
   indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps));
}

void Rhythm::clockTick()
{
   if (!firstTickDone) // ignore the first clock tick
   {
      firstTickDone = true;
      return;
   }

   if (stepSizeCounter.nextAndIsMaxValue())
      indexCounter.valueAndNext();
}

void Rhythm::clockReset()
{
   stepSizeCounter.reset();
   indexCounter.reset();

   firstTickDone = false;
   lastIndex = 0;
}

uint8_t Rhythm::getVelocity(const uint8_t& index) const
{
   if (0 == getNumberOfSteps())
      return 0;

   return velocities[index];
}

void Rhythm::changeVelocity(const uint8_t index, bool up)
{
   Variable::Integer<uint8_t, 10> var(velocities[index], 0, 127, false);
   var.change(up);

   Remember::Root::setUnsynced();
}

uint8_t Rhythm::currentIndex() const
{
   return indexCounter.getCurrentValue();
}

uint8_t Rhythm::currentVelocity()
{
   if (0 == getNumberOfSteps())
      return 0;

   uint8_t index = currentIndex();
   if (index != lastIndex)
   {
      lastIndex = index;
      return 0;
   }
   const float& value = getVelocity(index);

   return value;
}

uint8_t Rhythm::getNumberOfSteps() const
{
   return numberOfSteps;
}

void Rhythm::changeNumberOfSteps(bool longer)
{
   Variable::Integer<uint8_t> var(numberOfSteps, 0, maxLength, false);
   if (var.change(longer))
   {
      indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps));
      Remember::Root::setUnsynced();
   }
}

Tempo::Division Rhythm::getStepSize() const
{
   return stepSize;
}

void Rhythm::changeStepSize(bool longer)
{
   using StepSizeOrder = std::vector<Tempo::Division>;
   static const StepSizeOrder order = {Tempo::Division::Sixteenth, Tempo::Division::Eigth, Tempo::Division::Quarter, Tempo::Division::Bar};

   Variable::Enum<Tempo::Division> var(stepSize, order, true);

   var.change(longer);

   Remember::Root::setUnsynced();
   stepSizeCounter.setMaxValue(static_cast<uint8_t>(stepSize));
}

void Rhythm::copyFromRhythm(const Rhythm& other)
{
   for (uint8_t index = 0; index < maxLength; index++)
      velocities[index] = other.velocities[index];

   numberOfSteps = other.numberOfSteps;
   stepSize = other.stepSize;

   Remember::Root::setUnsynced();

   stepSizeCounter.setMaxValue(static_cast<uint8_t>(stepSize));
   indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps));
}

void Rhythm::clear()
{
   stepSize = Tempo::Division::Eigth;

   numberOfSteps = 0;
   Remember::Root::setUnsynced();

   stepSizeCounter.setMaxValue(static_cast<uint8_t>(stepSize));
   indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps));

   lastIndex = 0;

   // clear?
}
