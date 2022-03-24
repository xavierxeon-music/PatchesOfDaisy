#ifndef TempoWidgetH
#define TempoWidgetH

#include "ui_TempoWidget.h"
#include <QWidget>

namespace Imposter
{
   class DaisyPatch;
}

class TempoWidget : public QWidget, private Ui::TempoWidget
{
   Q_OBJECT
public:
   TempoWidget(QWidget* parent, Imposter::DaisyPatch* imposterPatch);

public slots:
   void slotSetExternalControls();

private:
   enum class Action
   {
      None,
      Reset,
      Step,
      Play
   };
private slots:
   void slotStepClicked();
   void slotStartPauseClicked();
   void slotTempoChanged(int value);
   void slotResetClicked();

private:
   Imposter::DaisyPatch* imposterPatch;
   Action action;
   float mSecsPerTick;
   float elapsedTime;
};

#endif // TempoWidgetH
