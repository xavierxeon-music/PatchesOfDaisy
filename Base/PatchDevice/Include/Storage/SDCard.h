#ifndef SDCardH
#define SDCardH

#include <Global.h>

#ifndef NON_DAISY_DEVICE

#include <daisy_patch.h>
using SDHandler = daisy::SdmmcHandler;

#else

#endif // NON_DAISY_DEVICE

// *************************************************** //
//  !! NEVER DO SD CARD OPERATIONS IN AUDIO LOOP !!   //
// *************************************************** //

class SDCard
{
public:
   static SDCard* the();

public:
   bool openRead(const std::string& fileName);
   bool openWrite(const std::string& fileName, bool append = false);
   void close();
   // clang-format off
   template <typename DataType> void load(DataType* data);
   template <typename DataType> void save(const DataType* data);
   // clang-format on
   void load(char* data, size_t size);
   void save(const char* data, size_t size);
   void saveString(const std::string& text);
   std::string compileSDStatus() const;
   bool isOpen(bool ignoreErrors = false) const;

private:
   enum class Operation
   {
      NotInitialised,
      Mount,
      LoadOpen,
      Read,
      SaveOpen,
      Write,
      Close
   };

#ifdef NON_DAISY_DEVICE
   enum class Error
   {
      None,
      FileOpen,
      Other
   };
#endif // NON_DAISY_DEVICE
private:
   SDCard();

private:
   void init();
   bool isMounted() const;

private:
#ifndef NON_DAISY_DEVICE
   SDHandler sdHandler;
   FRESULT error;
#else
   FILE* file;
   Error error;
#endif // NON_DAISY_DEVICE

   Operation lastOperation;
};

#include <SDCard.hpp>

#endif // SDCardH
