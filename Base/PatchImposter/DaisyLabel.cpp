#include "DaisyLabel.h"

DaisyLabel::DaisyLabel(QWidget* parent)
   : QLabel(parent)
{
   QPixmap logo(":/Imposter.svg");
   setPixmap(logo.scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation));

   setToolTip("[A] Back\n[D] Forward\n[K] Click\n[P] Press");
}

void DaisyLabel::mouseDoubleClickEvent(QMouseEvent* me)
{
   Q_UNUSED(me);
   emit signalDobuleClicked();
}
