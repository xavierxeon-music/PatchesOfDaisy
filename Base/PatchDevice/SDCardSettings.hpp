#ifndef SDCardSettingsHPP
#define SDCardSettingsHPP

#include <Storage/SDCardSettings.h>

#include <Abstract/AbstractPatch.h>
#include <Storage/SDCard.h>

SDCardSettings::SDCardSettings(Abstract::Patch* patch, const uint8_t& version)
   : FlashSettings(patch, version)
   , settingsFileName()
{
   settingsFileName = "Remember_" + patch->getAppName() + ".bin";
}

void SDCardSettings::load()
{
   const bool open = SDCard::the()->openRead(settingsFileName);
   if (!open)
      return FlashSettings::load();

   const size_t dataSize = sizeof(uint8_t) + compileDataSize();
   Remember::DataVector data(dataSize);

   SDCard::the()->load((char*)(&data[0]), dataSize);
   SDCard::the()->close();

   const uint8_t bufferVersion = data[0];
   if (bufferVersion != version)
      return;
   data.erase(data.begin()); // remove buffer version

   set(data);
}

void SDCardSettings::save()
{
   const bool open = SDCard::the()->openWrite(settingsFileName);
   if (!open)
      return FlashSettings::save();

   Remember::DataVector rawData = get();
   const size_t dataSize = rawData.size();

   rawData.insert(rawData.begin(), version); // add buffer version

   SDCard::the()->save((const char*)(&rawData[0]), dataSize);
   SDCard::the()->close();
}

#endif // SDCardSettingsHPP
