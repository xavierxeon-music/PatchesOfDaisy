#include <Abstract/AbstractSettings.h>

#include <Abstract/AbstractPatch.h>

Abstract::Settings::Settings(Patch* patch)
   : Remember::Root()
   , patch(patch)
   , saveTimer()
{
   saveTimer.start();
}

void Abstract::Settings::ensureSettignsValid()
{
   // assume this function gets called periodically
   // try to save (if necessary) only once every 10 seconds

   static const uint32_t maxDiff = 10 * Timer::uSecsPerSecond;
   const uint32_t diff = saveTimer.elapsed();

   if (diff == Timer::invalid || maxDiff <= diff)
   {
      if (!isUnsynced())
         return;

      save();

      saveTimer.start();
      setSynced();
   }
}
