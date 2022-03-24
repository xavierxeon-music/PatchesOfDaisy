#ifndef CompanionAppH
#define CompanionAppH

#include <Remember.h>

#include <MusicTools.h>
#include <RtMidi4.h>

class CompanionApp : public Remember::Root
{
public:
   CompanionApp();
   ~CompanionApp();

protected:
   void loadFromFile(const QString& fileName);
   void requestLoadFromDaisy();
   virtual void loadedFromDaisy();
   void saveToFile(const QString& fileName);
   void saveToDaisy();

   void delayedOpenOutput(bool verbose = false);
   void delayedOpenInput(bool verbose = false);

private:
   void dataFromDaisy(const Bytes& message);
   static void midiError(RtMidiError::Type type, const std::string& errorText, void* userData);
   static void midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData);

private:
   RtMidiOut output;
   RtMidiIn input;
};

#endif // CompanionAppH
