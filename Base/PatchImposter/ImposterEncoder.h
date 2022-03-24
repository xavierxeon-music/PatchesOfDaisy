#ifndef ImposterEncoderH
#define ImposterEncoderH

#include <QObject>

#include <QLabel>
#include <QPushButton>

namespace Imposter
{
   class Encoder : public QObject
   {
      Q_OBJECT
   public:
      Encoder();

   public: // daisy facade
      void Debounce();
      int32_t Increment() const;
      bool FallingEdge() const;
      bool Pressed() const;
      float TimeHeldMs() const;

   public: // other
      void setup(QPushButton* plusButton, QPushButton* minusButton, QPushButton* clickButton, QPushButton* pressButton);

   private:
      void plus();
      void minus();
      void click();
      void press();

   private:
      mutable int32_t incValue;
      mutable bool clicked;
      mutable bool pressed;
   };
} // namespace Imposter

#endif // ImposterEncoderH
