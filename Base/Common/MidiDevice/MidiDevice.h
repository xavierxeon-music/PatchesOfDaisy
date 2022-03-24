#ifndef MidiDeviceH
#define MidiDeviceH

#include <functional>

#include <Midi/MidiCommon.h>
#include <Music/Note.h>
#include <MusicTools.h>
#include <Remember.h>

#include "RtMidi4/RtMidi4.h"

class MidiDevice
{
public:
   using NoteOnFunction = std::function<void(const Note&, const Midi::Velocity&)>;
   using NoteOffFunction = std::function<void(const Note&)>;
   using ControllChangeFunction = std::function<void(const Midi::ControllerMessage&, const uint8_t&)>;
   using LoadFromDaisyFunction = std::function<void()>;

public:
   MidiDevice(Remember::Root* root = nullptr, const Midi::Channel& receiveChannel = 0);
   ~MidiDevice();

public:
   void initMidi(bool verbose = false);

   void sendNoteOn(const Midi::Channel& channel, const Note& note, const Midi::Velocity& velocity);
   void sendNoteOff(const Midi::Channel& channel, const Note& note);
   void sendControllerChange(const Midi::Channel& channel, const Midi::ControllerMessage& cotrollerMessage, const uint8_t& value);

   template <typename ClassType>
   void onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Note&, const Midi::Velocity&));

   template <typename ClassType>
   void onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Note&));

   template <typename ClassType>
   void onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const Midi::ControllerMessage&, const uint8_t&));

   template <typename ClassType>
   void onLoadedFromDaisy(ClassType* instance, void (ClassType::*functionPointer)());

   void requestLoadFromDaisy();
   void saveToDaisy();

private:
   void openOutput(bool verbose);
   void openInput(bool verbose);
   void dataFromDaisy(const Bytes& message);
   static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
   static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

private:
   Remember::Root* root;
   const Midi::Channel receiveChannel;

   RtMidiOut output;
   RtMidiIn input;

   NoteOnFunction receiveNoteOnFunction;
   NoteOffFunction receiveNoteOffFunction;
   ControllChangeFunction receiveControllChangeFunction;
   LoadFromDaisyFunction loadFromDaisyFunction;
};

#ifndef MidiDeviceHPP
#include "MidiDevice.hpp"
#endif // NOT MidiDeviceHPP

#endif // NOT MidiDeviceH
