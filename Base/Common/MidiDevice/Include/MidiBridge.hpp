#ifndef MidiBridgeHPP
#define MidiBridgeHPP

#include "MidiBridge.h"

template <typename ClassType>
void MidiBridge::onLoadedFromDaisy(ClassType* instance, void (ClassType::*functionPointer)())
{
   loadedFromDaisyFunction = std::bind(functionPointer, instance);
}

#endif // NOT MidiBridgeHPP
