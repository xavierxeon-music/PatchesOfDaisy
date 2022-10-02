#ifndef ColorPickerH
#define ColorPickerH

#include <QWidget>

#include <MusicTools.h>

class QLabel;

class ColorPicker : public QWidget
{
   Q_OBJECT

public:
   ColorPicker(QWidget* parent);

signals:
   void signalColorSelected(const Color& color);

public:
   Color getColor() const;
   void setColor(const Color& newColor);

private:
   using LabelMap = QMap<QLabel*, Color>;

private:
   bool eventFilter(QObject* object, QEvent* event);

private:
   LabelMap labelMap;
   Color color;
};

#endif // NOT ColorPickerH
