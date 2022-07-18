#include <Storage/SDCard.h>

#ifndef NON_DAISY_DEVICE
#include <fatfs.h>
#else
#include <Settings.h>
#include <QString>
#endif // NON_DAISY_DEVICE

SDCard* SDCard::the()
{
   static SDCard* theCard = nullptr;
   if (!theCard)
   {
      theCard = new SDCard();
      theCard->init();
   }
   return theCard;
}

SDCard::SDCard()
#ifndef NON_DAISY_DEVICE
   : sdHandler()
   , error(FR_OK)
#else
   : file(nullptr)
   , error(Error::None)
#endif
   , lastOperation(Operation::NotInitialised)
{
}

bool SDCard::openRead(const std::string& fileName)
{
#ifndef NON_DAISY_DEVICE
   if (FR_OK != error)
      return false;

   const BYTE mode = FA_READ | FA_OPEN_EXISTING;
   error = f_open(&SDFile, fileName.c_str(), mode);

   lastOperation = Operation::LoadOpen;
   return (FR_OK == error);
#else
   if (Error::None != error)
      return false;

   const std::string localFileName = ::Settings::path().toStdString() + "/" + fileName;
   file = fopen(localFileName.c_str(), "r");
   if (!file)
      error = Error::FileOpen;
   else
      error = Error::None;

   lastOperation = Operation::LoadOpen;
   return (Error::None == error);
#endif // NON_DAISY_DEVICE
}

bool SDCard::openWrite(const std::string& fileName, bool append)
{
#ifndef NON_DAISY_DEVICE
   if (FR_OK != error && FR_NO_FILE != error) // maybe no file on load
      return false;

   const BYTE mode = FA_WRITE | (append ? FA_OPEN_APPEND : FA_CREATE_ALWAYS);
   error = f_open(&SDFile, fileName.c_str(), mode);

   lastOperation = Operation::SaveOpen;
   return (FR_OK == error);
#else
   if (Error::None != error && Error::FileOpen != error) // maybe no file on load
      return false;

   const std::string localFileName = ::Settings::path().toStdString() + "/" + fileName;
   file = fopen(localFileName.c_str(), append ? "a" : "w");
   if (!file)
      error = Error::FileOpen;
   else
      error = Error::None;

   lastOperation = Operation::SaveOpen;
   return (Error::None == error);
#endif // NON_DAISY_DEVICE
}

void SDCard::close()
{
   if (!isOpen(true))
      return;

#ifndef NON_DAISY_DEVICE
   f_close(&SDFile);
#else
   if (file)
   {
      fclose(file);
      file = nullptr;
   }
#endif // NON_DAISY_DEVICE

   lastOperation = Operation::Close;
}

void SDCard::load(char* data, size_t size)
{
   if (!isMounted())
      return;

#ifndef NON_DAISY_DEVICE
   if (FR_OK != error)
      return;
#else
   if (Error::None != error)
      return;
#endif // NON_DAISY_DEVICE

   if (Operation::LoadOpen != lastOperation)
      return;

#ifndef NON_DAISY_DEVICE
   unsigned int bytesRead = 0;
   error = f_read(&SDFile, data, size, &bytesRead);
#else
   unsigned int bytesRead = fread(data, size, 1, file);
   if (0 == bytesRead)
      error = Error::Other;
#endif // NON_DAISY_DEVICE

   lastOperation = Operation::Read;
}

void SDCard::save(const char* data, size_t size)
{
   if (!isMounted())
      return;

#ifndef NON_DAISY_DEVICE
   if (FR_OK != error)
      return;
#else
   if (Error::None != error)
      return;
#endif // NON_DAISY_DEVICE

   if (Operation::SaveOpen != lastOperation && Operation::Write != lastOperation)
      return;

#ifndef NON_DAISY_DEVICE
   unsigned int bytesWritten = 0;
   error = f_write(&SDFile, data, size, &bytesWritten);
#else
   unsigned int bytesWritten = fwrite(data, size, 1, file);
   if (0 == bytesWritten)
      error = Error::Other;
#endif // NON_DAISY_DEVICE

   lastOperation = Operation::Write;
}

void SDCard::saveString(const std::string& text)
{
   save(text.c_str(), text.size() * sizeof(const char));
}

size_t SDCard::getFileSize()
{
#ifndef NON_DAISY_DEVICE
   return f_size(&SDFile);
#else
   fseek(file, 0, SEEK_END);
   const size_t fileSize = ftell(file);
   rewind(file);
#endif // NON_DAISY_DEVICE

   return fileSize;
}

void SDCard::jumpToStart()
{
#ifndef NON_DAISY_DEVICE
   f_rewind(&SDFile);
#else
   rewind(file);
#endif // NON_DAISY_DEVICE
}

std::string SDCard::compileSDStatus() const
{
   if (!isMounted())
      return "No SD";

   std::string sd_status = "SD:";
#ifndef NON_DAISY_DEVICE

   switch (error)
   {
      case FR_OK: // /(0) Succeeded
         sd_status += "OK";
         break;
      case FR_DISK_ERR: // (1) A hard error occurred in the low level disk I/O layer
         sd_status += "OK";
         break;
      case FR_INT_ERR: // (2) Assertion failed
         sd_status += "assert fail";
         break;
      case FR_NOT_READY: // (3) The physical drive cannot work
         sd_status += "not ready";
         break;
      case FR_NO_FILE: // (4) Could not find the file
         sd_status += "no file";
         break;
      case FR_NO_PATH: // (5) Could not find the path
         sd_status += "no path";
         break;
      case FR_INVALID_NAME: // (6) The path name format is invalid
         sd_status += "invalid name";
         break;
      case FR_DENIED: // (7) Access denied due to prohibited access or directory full
         sd_status += "access denied";
         break;
      case FR_EXIST: // (8) Access denied due to prohibited access
         sd_status += "file exists";
         break;
      case FR_INVALID_OBJECT: // (9) The file/directory object is invalid
         sd_status += "invalid object";
         break;
      case FR_WRITE_PROTECTED: // (10) The physical drive is write protected
         sd_status += "write potect";
         break;
      case FR_INVALID_DRIVE: // (11) The logical drive number is invalid
         sd_status += "invalid number";
         break;
      case FR_NOT_ENABLED: // (12) The volume has no work area
         sd_status += "no work area";
         break;
      case FR_NO_FILESYSTEM: // (13) There is no valid FAT volume
         sd_status += "no FAT volume";
         break;
      case FR_MKFS_ABORTED: // (14) The f_mkfs() aborted due to any problem
         sd_status += "MKFS aborted";
         break;
      case FR_TIMEOUT: // (15) Could not get a grant to access the volume within defined period
         sd_status += "timeout";
         break;
      case FR_LOCKED: // (16) The operation is rejected according to the file sharing policy
         sd_status += "locked";
         break;
      case FR_NOT_ENOUGH_CORE: // (17) LFN working buffer could not be allocated
         sd_status += "! core";
         break;
      case FR_TOO_MANY_OPEN_FILES: // (18) Number of open files > _FS_LOCK
         sd_status += "too many open";
         break;
      case FR_INVALID_PARAMETER: // (19) Given parameter is invalid
         sd_status += "invalid param";
         break;
      default:
         sd_status += "unkown";
         break;
   }
#else
   if (Error::None == error)
      sd_status += "OK";
   else if (Error::FileOpen == error)
      sd_status += "no file";
   else if (Error::Other == error)
      sd_status += "???";
#endif // NON_DAISY_DEVICE

   return sd_status;
}

bool SDCard::isOpen(bool ignoreErrors) const
{
   switch (lastOperation)
   {
      case Operation::LoadOpen:
      case Operation::Read:
      case Operation::SaveOpen:
      case Operation::Write:
         break;
      default:
         return false;
         break;
   }

#ifndef NON_DAISY_DEVICE
   if (!ignoreErrors && FR_OK != error)
      return false;
#else
   if (!ignoreErrors && Error::None != error)
      return false;
#endif // NON_DAISY_DEVICE

   return true;
}

void SDCard::init()
{
   if (lastOperation != Operation::NotInitialised)
      return;
#ifndef NON_DAISY_DEVICE

   // Init SD Card
   daisy::SdmmcHandler::Config sdConfig;
   sdConfig.Defaults();

   sdHandler.Init(sdConfig);
   dsy_fatfs_init();

   error = f_mount(&SDFatFS, SDPath, 1);
#endif // NON_DAISY_DEVICE
   lastOperation = Operation::Mount;
}

bool SDCard::isMounted() const
{
   if (Operation::NotInitialised == lastOperation)
      return false;
#ifndef NON_DAISY_DEVICE
   if (Operation::Mount == lastOperation && FR_OK != error)
      return false;
#else
   if (Operation::Mount == lastOperation && Error::None != error)
      return false;
#endif // NON_DAISY_DEVICE
   return true;
}
