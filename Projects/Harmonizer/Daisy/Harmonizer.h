#ifndef HarmonizerH
#define HarmonizerH

#include <Abstract/AbstractPatch.h>
#include <Remember.h>
#include <Storage/FlashSettings.h>

#include <Effect/Revoice.h>

#include "Common.h"
#include "FilterBank.h"

class Harmonizer : public Abstract::Patch, public FlashSettings
{
public:
   Harmonizer();

public:
   const Notes& getNotes() const;
   uint8_t getNumberOfVoces() const;
   void changeNumbertOfVoices(bool more);

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

private: // things to remember
   using NumberOfVoices_ = Remember::Value<uint8_t>;

private:
   FilterBank filters[2];
   Notes notes;
   Revoice revoice[2];
   NumberOfVoices_ numberOfVoices;
};

#endif // HarmonizerH
