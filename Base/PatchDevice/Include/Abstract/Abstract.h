#ifndef AbstractH
#define AbstractH

#include <Global.h>

enum class Encoder
{
   None,
   Forwad,
   Back,
   Clicked,
   LongPressed
};

struct Input
{
   // valid voltages from 0.0V to 5.0V
   float controlVoltages[4] = {0.0, 0.0, 0.0, 0.0};
   bool gates[2] = {false, false};
};

struct Output
{
   // valid voltages from 0.0V to 5.0V
   float controlVoltages[2] = {0.0, 0.0};
   bool gate = false;
   bool led = false;
};

enum class Alignment
{
   Left,
   Center,
   Right
};

#endif // AbstractH
