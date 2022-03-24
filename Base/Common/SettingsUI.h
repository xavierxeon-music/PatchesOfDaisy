#ifndef SettingsUIH
#define SettingsUIH

#include <JSONToolsAbstractSettings.h>

class SettingsUI : public JSONTools::AbstractSettings
{
public:
   SettingsUI(const QString& groupName = QString());
   ~SettingsUI();

public:
   static const QString& fileName();

private:
   const QJsonObject& content() const override;
   QJsonObject& contentRef() override;

private:
   static QJsonObject docObject;
   static quint32 useCount;
};

#endif // SettingsUIH
