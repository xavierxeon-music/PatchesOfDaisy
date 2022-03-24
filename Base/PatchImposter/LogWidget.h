#ifndef LogWidgetH
#define LogWidgetH

#include "ui_LogWidget.h"
#include <QWidget>

#include <QFile>

class LogWidget : public QWidget, private Ui::LogWidget
{
   Q_OBJECT
public:
   LogWidget(QWidget* parent);
   ~LogWidget();

private slots:
   void slotToggleLog(bool active);
   void slotSelectLogFile();

private:
   void logger(QtMsgType type, const QMessageLogContext& context, const QString& message);
   static void messageHook(QtMsgType type, const QMessageLogContext& context, const QString& message);

private:
   static LogWidget* me;
   QtMessageHandler systemHandler;
   QFile logFile;
};

#endif // LogWidgetH
