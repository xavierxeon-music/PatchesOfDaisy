#ifndef AppSettingsH
#define AppSettingsH

#include <JSONToolsAbstractSettings.h>

class AppSettings : public JSONTools::AbstractSettings
{
public:
   AppSettings(const QString& groupName = QString());
   ~AppSettings();

public:
   static const QString& fileName();

private:
   const QJsonObject& content() const override;
   QJsonObject& contentRef() override;

private:
   static QJsonObject docObject;
   static quint32 useCount;
};

#endif // NOT AppSettingsH
