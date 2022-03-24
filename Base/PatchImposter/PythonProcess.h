#ifndef PythonProcessH
#define PythonProcessH

#include <QProcess>

namespace Python
{
   class Process : public QObject
   {
      Q_OBJECT
   public:
      Process(QObject* parent, const QString& appName);
      ~Process();

   signals:
      void signalStarted();
      void signalAborted();
      void signalFinished(int exitCode, QProcess::ExitStatus exitStatus);
      void signalError();
      void signalOutput();

   public slots:
      void slotLauchchOrStopScript();

   public:
      QString getOutputBuffer();
      QString getErrorBuffer();

   private slots:
      void slotProcessError();
      void slotProcessOutput();

   private:
      void shutdown();

   private:
      QProcess* internal;
      QString appName;
      QString outputBuffer;
      QString errorBuffer;
   };
} // namespace Python

#endif // PythonProcessH
