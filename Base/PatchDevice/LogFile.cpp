#include <Storage/LogFile.h>

#include <Storage/SDCard.h>
#include <Tools/Convert.h>

LogFile::LogFile(const std::string& fileName, const uint8_t& decimalPlaces)
   : decimalPlaces(decimalPlaces)
   , fileName(fileName)
   , canBuffer(false)
   , buffer()
   , bufferIndex(0)
   , maxEntries(0)
   , linesWritten(0)
{
}

void LogFile::clear(const uint64_t maxNumberOfEntries)
{
   bufferIndex = 0;

   maxEntries = maxNumberOfEntries;
   linesWritten = 0;

   // create new file
   SDCard::the()->openWrite(fileName);
   SDCard::the()->saveString("LOG: " + fileName);
   canBuffer = SDCard::the()->isOpen();
   SDCard::the()->close();
}

void LogFile::flushToCard()
{
   if (0 == bufferIndex)
      return;

   // append to file
   SDCard::the()->openWrite(fileName, true);
   for (uint64_t index = 0; index < bufferIndex; index++)
   {
      const std::string& text = buffer[index];
      SDCard::the()->saveString(text);
   }
   SDCard::the()->close();

   bufferIndex = 0;
}

LogFile& LogFile::operator<<(const bool& value)
{
   std::string text = Convert::text(value) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const int& value)
{
   std::string text = Convert::text(value) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const uint8_t& value)
{
   std::string text = Convert::text(value) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const uint32_t& value)
{
   std::string text = Convert::text(value) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const uint64_t& value)
{
   std::string text = Convert::text(value) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const float& value)
{
   std::string text = Convert::text(value, decimalPlaces) + "\n";
   bufferData(text);
   return *this;
}

LogFile& LogFile::operator<<(const std::string& value)
{
   std::string text = value + "\n";
   bufferData(text);
   return *this;
}

void LogFile::bufferData(const std::string& text)
{
   if (!canBuffer)
      return;

   buffer[bufferIndex] = text;
   bufferIndex++;

   linesWritten++;
   if (0 != maxEntries && linesWritten >= maxEntries)
      canBuffer = false;
}
