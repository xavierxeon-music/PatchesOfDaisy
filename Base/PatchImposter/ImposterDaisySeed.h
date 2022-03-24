#ifndef ImposterDaisySeedH
#define ImposterDaisySeedH

#include <QObject>

#include <QRadioButton>

namespace Imposter
{
   class DaisySeed : public QObject
   {
      Q_OBJECT
   public:
      DaisySeed();

   public: // daisy facade
      void SetLed(bool on);

   public: // other
      void setup(QRadioButton* radioButton);

   public:
      bool ledOn;
      QRadioButton* radioButton;
   };
} // namespace Imposter

#endif // ImposterDaisySeedH
