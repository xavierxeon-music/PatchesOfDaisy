#ifndef HardwareWidgetH
#define HardwareWidgetH

#include "ui_HardwareWidget.h"
#include <QWidget>

#include <Abstract/Abstract.h>

namespace Imposter
{
   class DaisyPatch;
}

class HardwareWidget : public QWidget, private Ui::HardwareWidget
{
   Q_OBJECT

public:
   HardwareWidget(QWidget* parent, Imposter::DaisyPatch* imposterPatch);

public:
   void refreshDisplay();
   void updateState();
   void applyOutput(const Output& output);

private slots:
   void slotNextAudioStep();
   void slotNextNonAudioStep();

private:
   void keyReleaseEvent(QKeyEvent* ke) override;

private:
   Imposter::DaisyPatch* imposterPatch;
   QPixmap displayPixmap;
   QPixmap audioInPixmap[4];
   QPixmap audioOutPixmap[4];
};

#endif // HardwareWidgetH
