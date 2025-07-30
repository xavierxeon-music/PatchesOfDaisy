#ifndef RhythmH
#define RhythmH

#include <Remember.h>

#include <Music/Tempo.h>
#include <Tools/Counter.h>

class Rhythm : public Remember::Container
{
public:
   static constexpr uint8_t maxLength = 24;
   using Values = float[maxLength];

public:
   Rhythm();

public:
   void init();
   void clockTick();
   void clockReset();

   uint8_t getVelocity(const uint8_t& index) const;
   void changeVelocity(const uint8_t index, bool up);
   uint8_t currentIndex() const;
   uint8_t currentVelocity();

   uint8_t getNumberOfSteps() const;
   void changeNumberOfSteps(bool longer);

   Tempo::Division getStepSize() const;
   void changeStepSize(bool longer);

   void copyFromRhythm(const Rhythm& other);
   void clear();

private: // things to remeber
   using StepSize_ = Remember::Value<Tempo::Division>;
   using IntValue_ = Remember::Value<uint8_t>;
   using IntValueArray_ = Remember::ValueArray<uint8_t, maxLength>;

private:
   IntValueArray_ velocities;
   IntValue_ numberOfSteps;
   StepSize_ stepSize;

   Counter stepSizeCounter;
   Counter indexCounter;

   bool firstTickDone;
   uint8_t lastIndex;
};

#endif // RhythmH
