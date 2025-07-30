#ifndef FilterBankH
#define FilterBankH

#include <Effect/StateVariableFilter.h>

#include "Common.h"

class FilterBank
{
public:
   FilterBank();

public:
   void init(const float& sampleRate);
   void update(const float& resonance, const Notes& notes);
   float changeSound(const float& in);

private:
   StateVariableFilter filters[4];
};

#endif // NOT FilterBankH
