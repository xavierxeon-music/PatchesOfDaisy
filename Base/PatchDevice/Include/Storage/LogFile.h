#ifndef LogFileH
#define LogFileH

#include <string>
#include <vector>

// stored one value per line as text
class LogFile
{
public:
   LogFile(const std::string& fileName, const uint8_t& decimalPlaces = 2);

public:
   void clear(const uint64_t maxNumberOfEntries = 0);
   void flushToCard();

public:
   LogFile& operator<<(const bool& value);
   LogFile& operator<<(const int& value);
   LogFile& operator<<(const uint8_t& value);
   LogFile& operator<<(const uint32_t& value);
   LogFile& operator<<(const uint64_t& value);
   LogFile& operator<<(const float& value);
   LogFile& operator<<(const std::string& value);
   // clang-format off
   template <typename DataType> LogFile& operator<<(const std::vector<DataType>& valueVector);
   // clang-format on
private:
   using Buffer = std::vector<std::string>;

private:
   void bufferData(const std::string& text);

private:
   const uint8_t decimalPlaces;
   std::string fileName;

   bool canBuffer;
   Buffer buffer;
   uint64_t bufferIndex;

   uint64_t maxEntries;
   uint64_t linesWritten;
};

template <typename DataType>
LogFile& LogFile::operator<<(const std::vector<DataType>& valueVector)
{
   *this << valueVector.size();
   for (const DataType& data : valueVector)
      *this << data;

   return *this;
}

#endif // LogFileH
