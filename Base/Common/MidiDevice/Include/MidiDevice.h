#ifndef MidiDeviceH
#define MidiDeviceH

#include <functional>

#include <Midi/MidiCommon.h>
#include <Music/Note.h>
#include <MusicTools.h>

#include "../RtMidi4/RtMidi4.h"

class MidiDevice
{
public:
   using NoteOnFunction = std::function<void(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity)>;
   using NoteOffFunction = std::function<void(const Midi::Channel& channel, const Note& note)>;
   using ControllChangeFunction = std::function<void(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value)>;

public:
   MidiDevice(const std::string portName);
   virtual ~MidiDevice();

public:
   void initMidi(bool verbose = false);

   void sendNoteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity);
   void sendNoteOff(const Midi::Channel& channel, const Note& note);
   void sendControllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& controllerMessage, const uint8_t& value);

   template <typename ClassType>
   void onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Note&, const Midi::Velocity&));

   template <typename ClassType>
   void onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Note&));

   template <typename ClassType>
   void onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const Midi::Channel&, const Midi::ControllerMessage&, const uint8_t&));

protected:
   RtMidiOut output;
   RtMidiIn input;

private:
   void dataFromInput(const Bytes& message);
   void openOutput(bool verbose);
   void openInput(bool verbose);
   static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
   static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

private:
   const std::string portName;

   std::vector<NoteOnFunction> noteOnFunctionList;
   std::vector<NoteOffFunction> noteOffFunctionList;
   std::vector<ControllChangeFunction> controllChangeFunctionList;
};

#ifndef MidiDeviceHPP
#include "MidiDevice.hpp"
#endif // NOT MidiDeviceHPP

#endif // NOT MidiDeviceH
