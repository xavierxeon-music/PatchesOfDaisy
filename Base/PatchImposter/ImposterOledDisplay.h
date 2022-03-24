#ifndef ImposterOledDisplayH
#define ImposterOledDisplayH

#include <QPainter>
#include <QPixmap>

#include "ImposterFontDef.h"

class HardwareWidget;

namespace Imposter
{
   class DaisyPatch;

   class OledDisplay
   {
   public:
      OledDisplay();

   public: // daisy facade
      void Fill(bool on);
      void SetCursor(uint16_t x, uint16_t y);
      void Update();
      void DrawPixel(uint8_t x, uint8_t y, bool on);
      // the code in the following functions is copied from OledDisplay.h in libdaisy
      void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on);
      void DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on, bool fill);
      void DrawCircle(uint8_t x, uint8_t y, uint8_t radius, bool on);
      void DrawArc(uint8_t x, uint8_t y, uint8_t radius, int16_t start_angle, int16_t sweep, bool on);
      char WriteChar(char ch, FontDef font, bool on);
      char WriteString(const char* str, FontDef font, bool on);

   public: // other
      void setup(HardwareWidget* hardwwareWidget, QPixmap* pixmap);

   private:
      HardwareWidget* hardwwareWidget;
      QPixmap* pixmap;
      QPainter painter;
      uint16_t cursorX;
      uint16_t cursorY;
   };
} // namespace Imposter

#endif // ImposterOledDisplayH
