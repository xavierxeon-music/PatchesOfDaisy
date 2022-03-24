#include "AudioChannelAbstract.h"

#include <QPainter>
#include <QPixmap>

const uint16_t AudioChannel::Abstract::bufferSize = 320;
const Range::Mapper AudioChannel::Abstract::amplitudeMapper(-1.1, 1.1, 49, 0);

AudioChannel::Abstract::Abstract()
   : QObject(nullptr)
   , pixmap(nullptr)
   , buffer(bufferSize, 0.0)
   , bufferIndex(0)
{
}

AudioChannel::Abstract::~Abstract()
{

}

void AudioChannel::Abstract::setup(QPixmap* pixmap)
{
   this->pixmap = pixmap;

   pixmap->fill(QColor(0, 0, 0));
}

void AudioChannel::Abstract::updatePixmap(std::vector<float>& values)
{
   static const uint16_t scale = 4;
   const uint16_t updateSize = values.size() / scale;
   for (uint16_t x = 0; x < updateSize; x++)
   {
      buffer[bufferIndex] = values[x * scale];
      bufferIndex++;
      if (bufferSize == bufferIndex)
         bufferIndex = 0;
   }

   if (!pixmap)
      return;

   QPainter painter(pixmap);
   static const QBrush bgBrush(QColor(200, 200, 200));
   painter.fillRect(0, 0, bufferSize, 50, bgBrush);

   static const QPen black(QColor(0, 0, 0));
   painter.setPen(black);

   uint8_t lastY = 0;
   for (uint16_t x = 0; x < bufferSize; x++)
   {
      const float& value = buffer[x];
      const uint8_t y = static_cast<uint8_t>(amplitudeMapper(value));
      if (0 == x)
      {
         lastY = y;
         continue;
      }

      painter.drawLine(x - 1, lastY, x, y);
      lastY = y;
   }

   static const QPen yellow(QColor(255, 255, 0));
   painter.setPen(yellow);
   painter.drawLine(bufferIndex, 0, bufferIndex, 49);
}
