#ifndef SDCardSettingsH
#define SDCardSettingsH

#include <Storage/FlashSettings.h>

#include <Global.h>

class SDCardSettings : public FlashSettings
{
public:
   inline SDCardSettings(Abstract::Patch* patch, const uint8_t& version);

protected:
   inline void load() override;

private:
   inline void save() override;

private:
   std::string settingsFileName;
};

#include <SDCardSettings.hpp>

#endif // SDCardSettingsH
