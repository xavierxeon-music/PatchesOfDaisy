#ifndef CustomTableH
#define CustomTableH

#include <Remember.h>
#include <Sound/WaveTable.h>

#include <Music/Note.h>
#include <Sound/StandardTable.h>
#include <Sound/WaveTableMorpher.h>

#include "CvMapping.h"

class RandomWalkTables;

class CustomTable : public WaveTable::Morpher, public Remember::Container
{
public:
   static const uint8_t maxBlend;

public:
   CustomTable();

public:
   void init(RandomWalkTables* randomWalkTables);

   // input / output
   CvMapping* getCvMapping();
   float setCvAndGetFrequency(const float controlVoltages[4]);
   float valueByAngle(const float& angle) const override;

   // user interaction - waveform
   Standard::Waveform::Shape getWaveform() const;
   void changeWaveform(bool up);
   std::string getWaveformName() const;

   // user interaction offset note / voltage
   Note getOffsetNote();
   void changeOffsetNote(bool up);

   // user interaction - seed
   uint8_t getSeed() const;
   const uint8_t& getMappedSeed() const;
   void changeSeed(bool up);

   // user interaction - blend
   uint8_t getBlend() const;
   const float& getMappedBlend() const;
   void changeBlend(bool up);

private: // things to remeber
   using Wafeform_ = Remember::Value<Standard::Waveform::Shape>;
   using Value_ = Remember::Value<uint8_t>;
   using CvMapping_ = Remember::Ref<CvMapping>;
   using Voltage_ = Remember::Value<float>;

private:
   CvMapping_ cvMapping;

   Wafeform_ waveform;
   Voltage_ offsetVoltage;

   Value_ seed;
   uint8_t seedInternal;

   Value_ blend;

   Standard::Table standardTable;
   mutable RandomWalkTables* randomWalkTables;
};

#endif // CustomTableH
