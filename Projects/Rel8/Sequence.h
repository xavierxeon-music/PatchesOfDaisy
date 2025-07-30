#ifndef SequenceH
#define SequenceH

#include <Remember.h>

#include <Music/Tempo.h>
#include <Tools/Counter.h>

class Sequence : public Remember::Container
{
public:
   static const uint8_t pauseValue;

public:
   Sequence();

public:
   void init();
   void clockTick();
   void clockReset();

   uint8_t currentIndex() const;
   uint8_t currentStepValue(const Tempo& tempo);

   const uint8_t& getStepValue(const uint8_t& index) const;
   void changeStepValue(const uint8_t& index, bool up);

   const uint8_t& getNumberOfSteps() const;
   void changeNumberOfSteps(bool longer);

   const Tempo::Division& getStepSize() const;
   void changeStepSize(bool longer);

private: // things to remeber
   using StepSize_ = Remember::Value<Tempo::Division>;
   using NumberOfSteps_ = Remember::Value<uint8_t>;
   using StepValues_ = Remember::ValueArray<uint8_t, 8>;

private:
   NumberOfSteps_ numberOfSteps;
   StepSize_ stepSize;
   StepValues_ values;

   Counter stepCounter;
   Counter indexCounter;

   bool firstTick;
   uint8_t lastIndex;
};

#endif // SequenceH
