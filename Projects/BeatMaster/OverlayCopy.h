#ifndef OverlayCopyH
#define OverlayCopyH

#include <Abstract/AbstractPageOverlay.h>

#include <Music/Tempo.h>
#include <Tools/SquareSignal.h>

class Main;

class OverlayCopy : public Abstract::Page::Overlay
{
public:
   OverlayCopy(Main* main);

private:
   enum class CopyOperation
   {
      SeqIndexSource,
      BankIndexSource,
      SeqIndexTarget,
      BankIndexTarget,
      ExecuteCopy
   };

   enum class Scope
   {
      Single,
      All,
      Clear
   };

   struct Location
   {
      uint8_t midiChannel;
      uint8_t bankIndex;
      Scope scope;
   };

private:
   void render(OledDisplay* display) override;
   void renderGrid(OledDisplay* display, const uint8_t xOffset, const Location& location);
   State selected() override;
   State forward() override;
   State back() override;
   State clicked() override;
   State longPressed() override;
   void executeCopy();

private:
   Main* main;
   CopyOperation operation;
   Location source;
   Location target;
   SquareSignal blink;
};

#endif // OverlayCopyH
