#ifndef FlashSettingsHPP
#define FlashSettingsHPP

#include <Storage/FlashSettings.h>

#include <cassert>
#include <cstring>

#ifdef NON_DAISY_DEVICE
#include <Settings.h>
#include <QFile>
#endif // NON_DAISY_DEVICE

FlashSettings::FlashSettings(Abstract::Patch* patch, const uint8_t& version)
   : Abstract::Settings(patch)
   , version(version)
   , magicBlock()
{
   // create magic block
   std::string magicString = patch->getAppName();

   while (magicString.size() < sizeof(MagicBlock)) // might be smaller
      magicString = magicString + "&";
   magicString = magicString.substr(0, sizeof(MagicBlock)); // might be larger

   std::memcpy((uint8_t*)magicBlock, magicString.c_str(), sizeof(MagicBlock));
}

void FlashSettings::load()
{
#ifdef NON_DAISY_DEVICE
   (void)patch;

   // do not zero buffer, should contain random stuff
   const QString fileName = ::Settings::path() + "/Remember.flash";

   QFile file(fileName);
   if (file.open(QIODevice::ReadOnly))
   {
      const QByteArray content = file.readAll();
      file.close();

      std::memcpy(&flashBuffer, content.constData(), bufferSize);
   }

#endif // NON_DAISY_DEVICE

   uint8_t offset = 0;

   // test if first bytes in memory are our magicBlock
   for (uint8_t index = 0; index < sizeof(MagicBlock); index++)
   {
      if (flashBuffer[index] != magicBlock[index])
         return;
   }
   offset += sizeof(MagicBlock);

   // test if version numbers match
   const uint8_t bufferVersion = flashBuffer[sizeof(MagicBlock)];
   const bool equal = (bufferVersion == version);
   if (!equal)
      return;
   offset += sizeof(uint8_t);

   // if tests ok, load data

   uint32_t dataSize = 0;
   std::memcpy((uint8_t*)(&dataSize), &flashBuffer[offset], sizeof(uint32_t));
   offset += sizeof(uint32_t);

   Remember::DataVector data(dataSize);
   std::memcpy(&data[0], &flashBuffer[offset], dataSize);

   set(data);
}

void FlashSettings::save()
{
   Remember::DataVector data = get();
   const uint32_t dataSize = data.size();

#ifndef NON_DAISY_DEVICE
   uint32_t address = (uint32_t)flashBuffer;

   daisy::QSPIHandle& flash = patch->daisy.seed.qspi;
   flash.Erase(address, address + sizeof(MagicBlock) + sizeof(uint8_t) + dataSize);

   flash.Write(address, sizeof(MagicBlock), (uint8_t*)magicBlock);
   address += sizeof(MagicBlock);

   flash.Write(address, sizeof(uint8_t), (uint8_t*)&version);
   address += sizeof(uint8_t);

   flash.Write(address, sizeof(uint32_t), (uint8_t*)&dataSize);
   address += sizeof(uint32_t);

   flash.Write(address, dataSize, (uint8_t*)(&data[0]));
#else
   const QString fileName = ::Settings::path() + "/Remember.flash";
   //qDebug() << "save flash to" << fileName;

   QFile file(fileName);
   if (!file.open(QIODevice::WriteOnly))
      return;

   uint32_t padding = bufferSize;

   file.write((const char*)magicBlock, sizeof(MagicBlock));
   padding -= sizeof(MagicBlock);

   file.write((const char*)&version, sizeof(uint8_t));
   padding -= sizeof(uint8_t);

   file.write((const char*)&dataSize, sizeof(uint32_t));
   padding -= sizeof(uint32_t);

   file.write((const char*)(&data[0]), dataSize);
   padding -= dataSize;

   const QByteArray padData('x', padding);
   file.write(padData.constData(), padding);

   file.close();
#endif
}

#endif // FlashSettingsHPP
