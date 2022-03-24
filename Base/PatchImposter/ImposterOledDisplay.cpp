#include "ImposterOledDisplay.h"

#include "ImposterDaisyPatch.h"

#include "HardwareWidget.h"

static const QMap<bool, QColor> color = {{false, QColor(0, 0, 0)}, {true, QColor(255, 255, 255)}};

Imposter::OledDisplay::OledDisplay()
   : hardwwareWidget(nullptr)
   , pixmap(nullptr)
   , painter()
   , cursorX(0)
   , cursorY(0)
{

}

void Imposter::OledDisplay::Fill(bool on)
{
   if (!pixmap)
      return;

   painter.begin(pixmap);

   QBrush brush(color[on]);
   painter.fillRect(0, 0, pixmap->width(), pixmap->height(), brush);
}

void Imposter::OledDisplay::DrawPixel(uint8_t x, uint8_t y, bool on)
{
   if (!pixmap)
      return;

   QPen pen(color[on]);
   painter.setPen(pen);
   painter.drawPoint(x + 1, y + 1);
}

void Imposter::OledDisplay::DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on)
{
   int16_t deltaX = abs((int16_t)x2 - (int16_t)x1);
   int16_t deltaY = abs((int16_t)y2 - (int16_t)y1);
   int16_t signX = ((x1 < x2) ? 1 : -1);
   int16_t signY = ((y1 < y2) ? 1 : -1);
   int16_t error = deltaX - deltaY;
   int16_t error2;

   DrawPixel(x2, y2, on);

   while ((x1 != x2) || (y1 != y2))
   {
      DrawPixel(x1, y1, on);
      error2 = error * 2;
      if (error2 > -deltaY)
      {
         error -= deltaY;
         x1 += signX;
      }

      if (error2 < deltaX)
      {
         error += deltaX;
         y1 += signY;
      }
   }
}

void Imposter::OledDisplay::DrawRect(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, bool on, bool fill)
{
   if (fill)
   {
      for (uint8_t x = x1; x <= x2; x++)
      {
         for (uint8_t y = y1; y <= y2; y++)
         {
            DrawPixel(x, y, on);
         }
      }
   }
   else
   {
      DrawLine(x1, y1, x2, y1, on);
      DrawLine(x2, y1, x2, y2, on);
      DrawLine(x2, y2, x1, y2, on);
      DrawLine(x1, y2, x1, y1, on);
   }
}

void Imposter::OledDisplay::DrawCircle(uint8_t x, uint8_t y, uint8_t radius, bool on)
{
   DrawArc(x, y, radius, 0, 360, on);
}

void Imposter::OledDisplay::DrawArc(uint8_t x, uint8_t y, uint8_t radius, int16_t start_angle, int16_t sweep, bool on)
{
   // Values to calculate the circle
   int16_t t_x, t_y, err, e2;

   // Temporary values to speed up comparisons
   float t_sxy, t_syx, t_sxny, t_synx;
   float t_exy, t_eyx, t_exny, t_eynx;

   float start_angle_rad, end_angle_rad;
   float start_x, start_y, end_x, end_y;

   bool d1, d2, d3, d4;

   d1 = d2 = d3 = d4 = true;

   bool circle = false;

   if (sweep < 0)
   {
      start_angle += sweep;
      sweep = -sweep;
   }

   auto deg2rad = [](uint16_t degrees) -> float
   {
      return static_cast<float>(degrees) * 3.14 / 180.0;
   };

   start_angle_rad = deg2rad(start_angle);
   end_angle_rad = deg2rad(start_angle + sweep);

   start_x = cos(start_angle_rad) * radius;
   start_y = -sin(start_angle_rad) * radius;
   end_x = cos(end_angle_rad) * radius;
   end_y = -sin(end_angle_rad) * radius;

   // Check if start and endpoint are very near
   if ((end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y) < 2.0f)
   {
      if (sweep > 180)
         circle = true;
      else
         // Nothing to draw
         return;
   }

   t_x = -radius;
   t_y = 0;
   err = 2 - 2 * radius;

   do
   {
      if (!circle)
      {
         t_sxy = start_x * t_y;
         t_syx = start_y * t_x;
         t_sxny = start_x * -t_y;
         t_synx = start_y * -t_x;
         t_exy = end_x * t_y;
         t_eyx = end_y * t_x;
         t_exny = end_x * -t_y;
         t_eynx = end_y * -t_x;

         if (sweep > 180)
         {
            d1 = (t_sxy - t_synx < 0 || t_exy - t_eynx > 0);
            d2 = (t_sxy - t_syx < 0 || t_exy - t_eyx > 0);
            d3 = (t_sxny - t_syx < 0 || t_exny - t_eyx > 0);
            d4 = (t_sxny - t_synx < 0 || t_exny - t_eynx > 0);
         }
         else
         {
            d1 = (t_sxy - t_synx < 0 && t_exy - t_eynx > 0);
            d2 = (t_sxy - t_syx < 0 && t_exy - t_eyx > 0);
            d3 = (t_sxny - t_syx < 0 && t_exny - t_eyx > 0);
            d4 = (t_sxny - t_synx < 0 && t_exny - t_eynx > 0);
         }
      }

      if (d1)
         DrawPixel(x - t_x, y + t_y, on);
      if (d2)
         DrawPixel(x + t_x, y + t_y, on);
      if (d3)
         DrawPixel(x + t_x, y - t_y, on);
      if (d4)
         DrawPixel(x - t_x, y - t_y, on);

      e2 = err;
      if (e2 <= t_y)
      {
         t_y++;
         err = err + (t_y * 2 + 1);
         if (-t_x == t_y && e2 <= t_x)
         {
            e2 = 0;
         }
      }
      if (e2 > t_x)
      {
         t_x++;
         err = err + (t_x * 2 + 1);
      }
   }
   while (t_x <= 0);
}

char Imposter::OledDisplay::WriteChar(char ch, FontDef font, bool on)
{
   if (!pixmap)
      return 0;

   // Check if character is valid
   if (ch < 32 || ch > 126)
      return 0;

   // Check remaining space on current line
   if (pixmap->width() < (cursorX + font.FontWidth) || pixmap->height() < (cursorY + font.FontHeight))
   {
      // Not enough space on current line
      return 0;
   }

   // Use the font to write
   for (uint8_t i = 0; i < font.FontHeight; i++)
   {
      const uint32_t blockIndex = (ch - 32) * font.FontHeight + i;
      const uint32_t block = font.data[blockIndex];
      for (uint8_t j = 0; j < font.FontWidth; j++)
      {
         if ((block << j) & 0x8000)
         {
            DrawPixel(cursorX + j, (cursorY + i), on);
         }
         else
         {
            DrawPixel(cursorX + j, (cursorY + i), !on);
         }
      }
   }

   // The current space is now taken
   SetCursor(cursorX + font.FontWidth, cursorY);

   // Return written char for validation
   return ch;
}

char Imposter::OledDisplay::WriteString(const char* str, FontDef font, bool on)
{
   // Write until null-byte
   while (*str)
   {
      if (WriteChar(*str, font, on) != *str)
      {
         // Char could not be written
         return *str;
      }

      // Next char
      str++;
   }
   // Everything ok
   return *str;
}

void Imposter::OledDisplay::SetCursor(uint16_t x, uint16_t y)
{
   cursorX = x;
   cursorY = y;
}

void Imposter::OledDisplay::Update()
{
   if (!pixmap)
      return;

   painter.end();

   if (hardwwareWidget)
      hardwwareWidget->refreshDisplay();
}

void Imposter::OledDisplay::setup(HardwareWidget* hardwwareWidget, QPixmap* pixmap)
{
   this->hardwwareWidget = hardwwareWidget;
   this->pixmap = pixmap;
}
