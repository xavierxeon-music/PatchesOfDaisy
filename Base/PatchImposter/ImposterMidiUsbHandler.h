#ifndef ImposterMidiUsbHandlerH
#define ImposterMidiUsbHandlerH

#include <QObject>

#include <Midi/MidiVirtualInput.h>
#include <Midi/MidiVirtualOutput.h>

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
      void noteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity);
      void noteOff(const Midi::Channel& channel, const Note& note);
      void controllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value);

   private:
      Midi::Virtual::Input input;
      Midi::Virtual::Output output;
      QList<Imposter::MidiEvent> eventList;
   };
} // namespace Imposter

#endif // ImposterMidiUsbHandlerH
