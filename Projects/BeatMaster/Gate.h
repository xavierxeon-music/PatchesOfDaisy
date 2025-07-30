#ifndef GateH
#define GateH

#include <Blocks/Trapezoid.h>

class Gate : public Trapezoid
{
public:
   Gate();

public:
   bool getApplyToKnobs() const;
   void toggleApplyToKnobs();

private: // things to remeber
   using BoolValue_ = Remember::Value<bool>;

private:
   BoolValue_ applyToKnobs;
};

#endif // GateH
