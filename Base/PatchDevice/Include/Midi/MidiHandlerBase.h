#ifndef MidiHandlerBaseH
#define MidiHandlerBaseH

#include <functional>

#ifndef NON_DAISY_DEVICE
#include <daisy_patch.h>
using MidiEvent = daisy::MidiEvent;
#else
#include <ImposterMidiEvent.h>
using MidiEvent = Imposter::MidiEvent;
#endif // NON_DAISY_DEVICE

#include <Midi/MidiCommon.h>
#include <Music/Note.h>
#include <MusicTools.h>

namespace Midi
{
   namespace Handler
   {
      class Base
      {
      public:
         using NoteOnFunction = std::function<void(const Note&, const Velocity&)>;
         using NoteOffFunction = std::function<void(const Note&)>;
         using ControllChangeFunction = std::function<void(const ControllerMessage&, const uint8_t&)>;

      public:
         inline Base(const Channel& receiveChannel, Base* passThroughHandler);

      public:
         inline virtual void initMidi();

         inline void sendNoteOn(const Channel& channel, const Note& note, const Velocity& velocity);
         inline void sendNoteOff(const Channel& channel, const Note& note);
         inline void sendControllerChange(const Channel& channel, const ControllerMessage& cotrollerMessage, const uint8_t& value);
         virtual void sendBuffer(const Bytes& buffer) = 0;

         template <typename ClassType>
         inline void onReceiveNoteOn(ClassType* instance, void (ClassType::*functionPointer)(const Note&, const Velocity&));
         template <typename ClassType>
         inline void onReceiveNoteOff(ClassType* instance, void (ClassType::*functionPointer)(const Note&));
         template <typename ClassType>
         inline void onReceiveControllChange(ClassType* instance, void (ClassType::*functionPointer)(const ControllerMessage&, const uint8_t&));

      protected:
         virtual void pollMidiReceive() = 0; // IF you want to receive midi, then call this function periodically, fx. in nonAudioLoop
         inline void handleMidiMessage(MidiEvent event);

      private:
         const Channel& receiveChannel;
         Base* passThroughHandler;
         NoteOnFunction receiveNoteOnFunction;
         NoteOffFunction receiveNoteOffFunction;
         ControllChangeFunction receiveControllChangeFunction;
      };
   } // namespace Handler
} // namespace Midi

#include "MidiHandlerBase.hpp"

#endif // MidiHandlerBaseH
