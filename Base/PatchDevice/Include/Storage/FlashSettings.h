#ifndef FlashSettingsH
#define FlashSettingsH

#include <Abstract/AbstractSettings.h>

#include <Abstract/AbstractPatch.h>

#ifndef NON_DAISY_DEVICE

#include <daisy_patch.h>

#else

#define DSY_QSPI_BSS

#endif // NON_DAISY_DEVICE

static constexpr uint32_t bufferSize = 64 * 1024; // 64 kB
static uint8_t DSY_QSPI_BSS flashBuffer[bufferSize];

class FlashSettings : public Abstract::Settings
{
public:
   inline FlashSettings(Abstract::Patch* patch, const uint8_t& version);

public:
   inline void load() override;

protected:
   inline void save() override;

protected:
   const uint8_t version;

private:
   using MagicBlock = char[16];

private:
   MagicBlock magicBlock;
};

#include <FlashSettings.hpp>

#endif // FlashSettingsH
