#ifndef ImposterGateInH
#define ImposterGateInH

#include <QObject>

#include <QRadioButton>

namespace Imposter
{
   class GateIn : public QObject
   {
      Q_OBJECT

   public:
      GateIn();

   public: // daisy facade
      bool Trig();

   public: // other
      void setup(QRadioButton* radioButton);
      void activateRemote(bool on);

   private:
      bool active;
      QRadioButton* radioButton;
   };
} // namespace Imposter

#endif // ImposterGateInH
