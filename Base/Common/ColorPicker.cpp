#include "ColorPicker.h"

#include <QGridLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

ColorPicker::ColorPicker(QWidget* parent)
   : QWidget(parent)
   , labelMap()
   , color{0, 0, 0}
{

   QGridLayout* masterLayout = new QGridLayout(this);
   masterLayout->setContentsMargins(0, 0, 0, 0);
   masterLayout->setSpacing(1);

   auto addLabel = [&](const Color& color, int row, int column)
   {
      QLabel* label = new QLabel(this);

      QPixmap pixmap(12, 12);
      pixmap.fill(QColor(color.red, color.green, color.blue));
      label->setPixmap(pixmap);

      label->installEventFilter(this);
      labelMap[label] = color;

      masterLayout->addWidget(label, row, column);
   };

   static const QList<Color> colorList = {Color{255, 0, 0},
                                          Color{0, 255, 0},
                                          Color{0, 0, 255},
                                          Color{255, 255, 0},
                                          Color{0, 255, 255},
                                          Color{255, 0, 255},
                                          Color{255, 255, 255}};
   static const QList<float> shadeList = {1.0, 0.95, 0.9, 0.85, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3};

   addLabel(Color{0, 0, 0}, shadeList.size() - 1, 0);

   for (uint8_t index = 0; index < colorList.size(); index++)
   {
      for (uint8_t y = 0; y < shadeList.size(); y++)
      {
         const float shade = shadeList[y];

         const uint8_t red = static_cast<uint8_t>(colorList[index].red * shade);
         const uint8_t green = static_cast<uint8_t>(colorList[index].green * shade);
         const uint8_t blue = static_cast<uint8_t>(colorList[index].blue * shade);

         addLabel(Color{red, green, blue}, y, index + 1);
      }
   }
}

Color ColorPicker::getColor() const
{
   return color;
}

void ColorPicker::setColor(const Color& newColor)
{
   color = newColor;

   for (LabelMap::Iterator it = labelMap.begin(); it != labelMap.end(); it++)
   {
      QLabel* label = it.key();
      if (color == it.value())
      {
         label->setFrameShape(QFrame::Box);
         label->setFrameShadow(QFrame::Sunken);
      }
      else
      {
         label->setFrameShape(QFrame::NoFrame);
         label->setFrameShadow(QFrame::Plain);
      }
   }
}

bool ColorPicker::eventFilter(QObject* object, QEvent* event)
{
   const bool result = QWidget::eventFilter(object, event);

   QLabel* label = qobject_cast<QLabel*>(object);
   if (labelMap.contains(label))
   {
      if (QEvent::MouseButtonPress == event->type())
      {
         const Color& newColor = labelMap.value(label);
         setColor(newColor);
         emit signalColorSelected(newColor);
      }
   }

   return result;
}
