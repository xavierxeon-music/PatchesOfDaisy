#include "TempoWidget.h"

#include "ImposterDaisyPatch.h"
#include "Settings.h"

TempoWidget::TempoWidget(QWidget* parent, Imposter::DaisyPatch* imposterPatch)
   : QWidget(parent)
   , imposterPatch(imposterPatch)
   , action(Action::None)
   , mSecsPerTick(1.0)
   , elapsedTime(0.0)
{
   setupUi(this);

   connect(stepButton, &QPushButton::clicked, this, &TempoWidget::slotStepClicked);
   connect(playPauseButton, &QPushButton::clicked, this, &TempoWidget::slotStartPauseClicked);
   connect(bpmSpin, &QSpinBox::valueChanged, this, &TempoWidget::slotTempoChanged);
   connect(resetButton, &QPushButton::clicked, this, &TempoWidget::slotResetClicked);

   playPauseButton->setCheckable(true);

   {
      Settings tempoSettings("tempo");
      const int beatsPerMinute = tempoSettings.integer("bpm", 120);
      mSecsPerTick = float(60 * 1000) / float(4 * beatsPerMinute);
   }
}

void TempoWidget::slotSetExternalControls()
{
   imposterPatch->gate_input[0].activateRemote(false);
   imposterPatch->gate_input[1].activateRemote(false);

   if (Action::Reset == action)
   {
      imposterPatch->gate_input[1].activateRemote(true);
      elapsedTime = 0.0;
      action = Action::None;
   }
   else if (Action::Step == action)
   {
      imposterPatch->gate_input[0].activateRemote(true);
      action = Action::None;
   }
   else if (Action::Play == action)
   {
      const float callBackIntervall = 1000.0 / imposterPatch->AudioCallbackRate();
      elapsedTime += callBackIntervall;

      if (elapsedTime >= mSecsPerTick)
      {
         imposterPatch->gate_input[0].activateRemote(true);
         elapsedTime = 0.0;
      }
   }
}

void TempoWidget::slotStepClicked()
{
   action = Action::Step;
}

void TempoWidget::slotStartPauseClicked()
{
   if (Action::Play == action)
   {
      action = Action::None;
      playPauseButton->setIcon(QIcon(":/Play.svg"));
   }
   else if (Action::None == action)
   {
      action = Action::Play;
      playPauseButton->setIcon(QIcon(":/Pause.svg"));
   }
}

void TempoWidget::slotTempoChanged(int value)
{
   Settings tempoSettings("tempo");
   tempoSettings.write("bpm", value);

   mSecsPerTick = float(60 * 1000) / float(4 * value);
}

void TempoWidget::slotResetClicked()
{
   action = Action::Reset;
}
