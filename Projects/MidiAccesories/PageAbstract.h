#ifndef PageAbstractH
#define PageAbstractH

#include <Abstract/AbstractPageBase.h>

#include <Global.h>
#include <Midi/MidiHandlerBase.h>

class MidiAccesories;

class PageAbstract : public Abstract::Page::Base
{
public:
   using List = std::vector<PageAbstract*>;

public:
   PageAbstract(Midi::Handler::Base* midiHandler);

public:
   virtual void advance(const uint16_t& msCallBackIntervall);

protected:
   virtual void initDevice();

protected:
   Midi::Handler::Base* midiHandler;

private:
   State clicked() override;
};

#endif // PageAbstractH
