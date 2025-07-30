#include "Channel.h"

const std::vector<std::string> Channel::names = {"A", "B", "C", "D", "E", "F", "G", "H"};

Channel::Channel(bool front)
   : rootNote()
   , selection(0)
   , front(front)
{
}

void Channel::compile(const Input& input)
{
   const uint8_t index = front ? 0 : 2;

   rootNote = Note::fromVoltage(5.0 * input.knobs[index + 0]);

   selection = static_cast<uint8_t>(8.0 * input.knobs[index + 1]);
   if (selection > 7)
      selection = 7;
}
