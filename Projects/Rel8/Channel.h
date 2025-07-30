#ifndef ChannelH
#define ChannelH

#include <Global.h>

#include <Abstract/Abstract.h>
#include <Music/Note.h>

class Channel
{
public:
   static const std::vector<std::string> names;

public:
   Channel(bool front);

public:
   void compile(const Input& input);

public:
   Note rootNote;
   uint8_t selection;

private:
   bool front;
};

#endif // ChannelH
