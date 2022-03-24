#include "LogWidget.h"

#include <QFileDialog>

#include "Settings.h"

LogWidget* LogWidget::me = nullptr;

LogWidget::LogWidget(QWidget* parent)
   : QWidget(parent)
   , systemHandler(nullptr)
   , logFile()
{
   me = this;
   systemHandler = qInstallMessageHandler(&LogWidget::messageHook);

   setupUi(this);
   logEnableButton->setCheckable(true);

   connect(logEnableButton, &QAbstractButton::clicked, this, &LogWidget::slotToggleLog);
   connect(selectLogButton, &QAbstractButton::clicked, this, &LogWidget::slotSelectLogFile);

   {
      Settings logSettings("log");
      const QString fileName = logSettings.string("file");
      logLabel->setText(fileName);
   }
}

LogWidget::~LogWidget()
{
   me = nullptr;

   if (logFile.isOpen())
      logFile.close();
}

void LogWidget::slotToggleLog(bool active)
{
   if (active)
   {
      logFile.setFileName(logLabel->text());
      logFile.open(QIODevice::WriteOnly);
   }
   else
   {
      if (logFile.isOpen())
         logFile.close();
   }
}

void LogWidget::slotSelectLogFile()
{
   QString fileName = QFileDialog::getSaveFileName(this, "Log File", QString(), "*.txt");
   if (fileName.isEmpty())
      return;

   logLabel->setText(fileName);

   Settings logSettings("log");
   logSettings.write("file", fileName);
}

void LogWidget::logger(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
   systemHandler(type, context, message);

   if (!logFile.isOpen())
      return;

   const QByteArray data = message.toLatin1() + "\n";
   logFile.write(data);
}

void LogWidget::messageHook(QtMsgType type, const QMessageLogContext& context, const QString& message)
{
   if (!me)
      return;

   me->logger(type, context, message);
}
