#include <Abstract/AbstractPage.h>

const bool Abstract::Page::White = true;
const bool Abstract::Page::Black = false;

Abstract::Page::Page()
   : mode(Mode::Neutral)
{
}

void Abstract::Page::render(OledDisplay* display)
{
   (void)display;
}

Abstract::Page::State Abstract::Page::selected()
{
   // do nothing
   return State::Turn;
}

Abstract::Page::State Abstract::Page::forward()
{
   // do nothing
   return State::Turn;
}

Abstract::Page::State Abstract::Page::back()
{
   // do nothing
   return State::Turn;
}

Abstract::Page::State Abstract::Page::clicked()
{
   // do nothing
   return State::Turn;
}

Abstract::Page::State Abstract::Page::longPressed()
{
   // do nothing
   return State::Turn;
}

bool Abstract::Page::skip()
{
   return false;
}

void Abstract::Page::write(OledDisplay* display, const uint8_t x, const uint8_t y, const std::string& text, const FontDef& font, bool color)
{
   display->SetCursor(x, y);
   display->WriteString(text.c_str(), font, color);
}

uint8_t Abstract::Page::compileLeftX(const uint8_t x, const std::string& text, const FontDef& fontDef, const Alignment& alignment)
{
   const uint8_t textWidth = text.size() * fontDef.FontWidth;

   uint8_t xLeft = x;
   if (Alignment::Right == alignment)
      xLeft = x - textWidth;
   else if (Alignment::Center == alignment)
      xLeft = x - (textWidth / 2);

   return xLeft;
}

