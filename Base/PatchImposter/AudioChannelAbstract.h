#ifndef AudioChannelAbstractH
#define AudioChannelAbstractH

#include <QObject>

#include <Tools/Range.h>

namespace AudioChannel
{
   class Abstract : public QObject
   {
      Q_OBJECT
   public:
      using Buffer = std::vector<float>;

   public:
      Abstract();
      virtual ~Abstract();

   public:
      virtual void setup(QPixmap* pixmap);

   protected:
      void updatePixmap(std::vector<float>& values);

   private:
      static const uint16_t bufferSize;
      static const Range::Mapper amplitudeMapper;
      QPixmap* pixmap;
      Buffer buffer;
      uint16_t bufferIndex;
   };
} // namespace AudioChannel

#endif // AudioChannelAbstractH
