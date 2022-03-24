#ifndef ImposterAnalogControlH
#define ImposterAnalogControlH

#include <QObject>

#include <QDial>
#include <QLabel>

namespace Imposter
{
   class AnalogControl : public QObject
   {
      Q_OBJECT
   public:
      AnalogControl();

   public: // daisy facade
      float Value() const;

   public: // other
      void init(uint8_t channel);
      void setup(QDial* dial, QLabel* valueLabel);
      void setRemote(const float& value);

   private:
      void dialChanged(double value);

   private:
      QString name;
      QDial* dial;
      QLabel* valueLabel;
      mutable float currentKnob;
      float currentRemote;
   };
} // namespace Imposter

#endif // EMULATORANALOGCONTROL_H
