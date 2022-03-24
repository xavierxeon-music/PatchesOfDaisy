#ifndef ImposterMidiUsbHandlerH
#define ImposterMidiUsbHandlerH

#include <QObject>

#include <RtMidi4.h>

#include <ImposterMidiEvent.h>

namespace Imposter
{
   class MidiUsbHandler : public QObject
   {
      Q_OBJECT
   public:
      MidiUsbHandler();
      ~MidiUsbHandler() override;

   public:
      void StartReceive();
      void Listen();
      bool HasEvents() const;
      MidiEvent PopEvent();
      void SendMessage(uint8_t* bytes, size_t size);

   private:
      Q_INVOKABLE void dataReceived(const Bytes& message);
      static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
      static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

   private:
      RtMidiOut output;
      RtMidiIn input;
      QList<Imposter::MidiEvent> eventList;
   };
} // namespace Imposter

#endif // ImposterMidiUsbHandlerH
