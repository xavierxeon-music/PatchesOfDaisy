#ifndef FileSettingsH
#define FileSettingsH

#include <JSONToolsAbstractSettings.h>

class FileSettings : public JSONTools::AbstractSettings
{
public:
   FileSettings(const QString& groupName = QString());
   ~FileSettings();

public:
   static const QString& getFileName();
   static void setFileName(const QString& newFileName);

private:
   const QJsonObject& content() const override;
   QJsonObject& contentRef() override;

private:
   static QString fileName;
   static QJsonObject docObject;
   static quint32 useCount;
};

#endif // NOT FileSettingsH
