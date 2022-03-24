#ifndef DaisyLabelH
#define DaisyLabelH

#include <QLabel>

class DaisyLabel : public QLabel
{
   Q_OBJECT
public:
   DaisyLabel(QWidget* parent);

signals:
   void signalDobuleClicked();

private:
   void mouseDoubleClickEvent(QMouseEvent* me) override;
};

#endif // DaisyLabelH
