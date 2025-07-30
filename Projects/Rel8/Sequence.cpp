#include "Sequence.h"

#include <Abstract/AbstractSettings.h>
#include <Tools/Variable.h>

const uint8_t Sequence::pauseValue = 24;

Sequence::Sequence()
   : Remember::Container()
   , numberOfSteps(this, 0)
   , stepSize(this, Tempo::Division::Eigth)
   , values(this, {0, 0, 0, 0, 0, 0, 0, 0})
   , stepCounter(1)
   , indexCounter(8)
   , firstTick(false)
   , lastIndex(0)
{
}

void Sequence::init()
{
   stepCounter.setMaxValue(static_cast<uint8_t>(stepSize.constValue()));
   indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps.constValue()));
}

void Sequence::clockTick()
{
   if (firstTick) // ignore the first clock tick
   {
      firstTick = false;
      return;
   }

   if (stepCounter.nextAndIsMaxValue())
      indexCounter.valueAndNext();
}

void Sequence::clockReset()
{
   stepCounter.reset();
   indexCounter.reset();

   firstTick = true;
   lastIndex = 0;
}

uint8_t Sequence::currentIndex() const
{
   return indexCounter.getCurrentValue();
}

uint8_t Sequence::currentStepValue(const Tempo& tempo)
{
   if (0 == getNumberOfSteps())
      return pauseValue;

   const float percentage = tempo.getPercentage(getStepSize());
   if (percentage > 0.5)
      return pauseValue;

   uint8_t index = currentIndex();
   /*
   if (index != lastIndex)
   {
      lastIndex = index;
      return pauseValue;
   }
   */

   const uint8_t& value = getStepValue(index);
   return value;
}

const uint8_t& Sequence::getStepValue(const uint8_t& index) const
{
   return values[index].constValue();
}

void Sequence::changeStepValue(const uint8_t& index, bool up)
{
   Variable::Integer<uint8_t> var(values[index].refValue(), 0, pauseValue, false);
   if (var.change(up))
   {
      Remember::Root::setUnsynced();
   }
}

const uint8_t& Sequence::getNumberOfSteps() const
{
   return numberOfSteps.constValue();
}

void Sequence::changeNumberOfSteps(bool longer)
{
   Variable::Integer<uint8_t> var(numberOfSteps.refValue(), 0, 8, false);
   if (var.change(longer))
   {
      indexCounter.setMaxValue(static_cast<uint64_t>(numberOfSteps.constValue()));
      Remember::Root::setUnsynced();
   }
}

const Tempo::Division& Sequence::getStepSize() const
{
   return stepSize.constValue();
}

void Sequence::changeStepSize(bool longer)
{
   using StepSizeOrder = std::vector<Tempo::Division>;
   static const StepSizeOrder order = {Tempo::Division::Sixteenth, Tempo::Division::Eigth, Tempo::Division::Quarter, Tempo::Division::Bar};

   Variable::Enum<Tempo::Division> var(stepSize.refValue(), order, true);

   var.change(longer);

   Remember::Root::setUnsynced();
   stepCounter.setMaxValue(static_cast<uint8_t>(stepSize.constValue()));
}
