#ifndef MidiAccesoriesH
#define MidiAccesoriesH

// base
#include <Abstract/AbstractPatch.h>

#include <Midi/MidiHandlerInternal.h>

#include "PageAbstract.h"

class MidiAccesories : public Abstract::Patch, public Midi::Handler::Internal
{
public:
   MidiAccesories();

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;

private:
   PageAbstract::List deviceList;
};

#endif // MidiAccesoriesH
