#ifndef MidiBridgeHPP
#define MidiBridgeHPP

#include "MidiBridge.h"

template <typename ClassType>
void Midi::Bridge::onLoadedFromDaisy(ClassType* instance, void (ClassType::*functionPointer)())
{
   loadedFromDaisyFunction = std::bind(functionPointer, instance);
}

#endif // NOT MidiBridgeHPP
