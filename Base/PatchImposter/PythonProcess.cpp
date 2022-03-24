#include "PythonProcess.h"

#include <QFileInfo>
#include <QTimer>

#include "Settings.h"

Python::Process::Process(QObject* parent, const QString& appName)
   : QObject(parent)
   , internal(nullptr)
   , appName(appName)
   , outputBuffer()
   , errorBuffer()
{
   internal = new QProcess(this);

   connect(internal, &QProcess::started, this, &Process::signalStarted);
   connect(internal, &QProcess::finished, this, &Process::signalFinished);

   connect(internal, &QProcess::readyReadStandardError, this, &Process::slotProcessError);
   connect(internal, &QProcess::readyReadStandardOutput, this, &Process::slotProcessOutput);
}

Python::Process::~Process()
{
   if (QProcess::NotRunning == internal->state())
      return;

   shutdown();
}

void Python::Process::slotLauchchOrStopScript()
{
   if (QProcess::NotRunning == internal->state())
   {
      Settings pythonAppSettings("python", true);
      const QString executable = pythonAppSettings.string("executable");
      if (executable.isEmpty() || !QFileInfo::exists(executable))
      {
         errorBuffer.append("set a valid python executable in the settings");
         emit signalAborted();
         return;
      }

      Settings pythonSettings("python");
      QString fileName = pythonSettings.string("script");
      if (fileName.isEmpty())
         fileName = appName + ".py";

      if (!QFileInfo::exists(fileName))
      {
         errorBuffer.append(fileName + " does not exist");
         emit signalAborted();
         return;
      }

      qDebug() << "start python process with script" << fileName;

      internal->start(executable, QStringList() << fileName, QIODevice::ReadWrite);
      internal->waitForStarted();
   }
   else
   {
      shutdown();
   }
}

QString Python::Process::getOutputBuffer()
{
   const QString output = outputBuffer;
   outputBuffer.clear();

   return output;
}

QString Python::Process::getErrorBuffer()
{
   const QString error = errorBuffer;
   errorBuffer.clear();

   return error;
}

void Python::Process::slotProcessError()
{
   const QString error = internal->readAllStandardError();
   outputBuffer.append(error);
   emit signalOutput();
}

void Python::Process::slotProcessOutput()
{
   const QString output = internal->readAllStandardOutput();
   errorBuffer.append(output);
   emit signalError();
}

void Python::Process::shutdown()
{
   internal->terminate(); // first try the nice way

   if (!internal->waitForFinished(1000))
   {
      internal->kill();
      internal->waitForFinished(1000);
   }

   qDebug() << "closed python process";
}
