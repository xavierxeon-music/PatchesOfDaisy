#ifndef SettingsH
#define SettingsH

#include <JSONToolsAbstractSettings.h>

class Settings : public JSONTools::AbstractSettings
{
public:
   Settings(const QString& groupName = QString(), bool appSettings = false);
   ~Settings();

public:
   static QString path();
   QString fileName() const;

private:
   struct StaticData
   {
      QJsonObject docObject;
      quint32 useCount;
   };
   using Keeper = QMap<bool, StaticData>;

private:
   const QJsonObject& content() const override;
   QJsonObject& contentRef() override;

private:
   static Keeper keeper;
   bool appSettings;
};

#endif // SettingsH
