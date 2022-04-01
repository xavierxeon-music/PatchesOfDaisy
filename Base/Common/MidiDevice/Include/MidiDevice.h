#ifndef MidiDeviceH
#define MidiDeviceH

#include <Midi/MidiInterface.h>

#include "../RtMidi4/RtMidi4.h"

namespace Midi
{
   class Device : public Interface
   {

   public:
      Device(const QString& portName);
      virtual ~Device();

   public:
      void initMidi(bool verbose = false) override;

   protected:
      RtMidiOut output;
      RtMidiIn input;

   private:
      void sendBuffer(const Bytes& buffer) override;
      void dataFromInput(const Bytes& message);
      void openOutput(bool verbose);
      void openInput(bool verbose);
      static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
      static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

   private:
      const std::string portName; // rt midi does not us QString
   };
} // namespace Midi


#endif // NOT MidiDeviceH
