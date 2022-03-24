#ifndef AbstractSettingsH
#define AbstractSettingsH

#include <Remember.h>

#include <Global.h>

#include <Midi/MidiCommon.h>
#include <Midi/MidiHandlerBase.h>
#include <Tools/Timer.h>

namespace Abstract
{
   class Patch;

   class Settings : public Remember::Root
   {
   public:
      Settings(Patch* patch);

   public:
      void ensureSettignsValid();
      virtual void load() = 0; // must call load in constructor      

   protected:
      virtual void save() = 0;

   protected:
      Patch* patch;

   private:
      Timer saveTimer;
   };
} // namespace Abstract

#endif // AbstractSettingsH
