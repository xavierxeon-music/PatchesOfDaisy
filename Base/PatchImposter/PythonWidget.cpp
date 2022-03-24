#include "PythonWidget.h"

#include <QFileDialog>

#include "ImposterDaisyPatch.h"
#include "Settings.h"

Python::Widget::Widget(QWidget* parent, Imposter::DaisyPatch* imposterPatch)
   : QWidget(parent)
   , process(nullptr)
{
   setupUi(this);
   process = new Process(this, imposterPatch->getAppName());

   connect(pythonExecutableSelectButton, &QAbstractButton::clicked, this, &Widget::slotChooseInterpreter);
   connect(selectScriptButton, &QAbstractButton::clicked, this, &Widget::slotSelectScript);

   scriptButton->setCheckable(true);
   connect(scriptButton, &QAbstractButton::clicked, process, &Process::slotLauchchOrStopScript);

   connect(process, &Process::signalStarted, this, &Widget::slotProcessStarted);
   connect(process, &Process::signalAborted, this, &Widget::slotProcessAborted);
   connect(process, &Process::signalFinished, this, &Widget::slotProcessFinished);
   connect(process, &Process::signalError, this, &Widget::slotProcessError);
   connect(process, &Process::signalOutput, this, &Widget::slotProcessOutput);

   {
      Settings pythonAppSettings("python", true);
      const QString executable = pythonAppSettings.string("executable");
      pythonExecutableEdit->setText(executable);
   }

   {
      Settings pythonSettings("python");
      QString fileName = pythonSettings.string("script");
      if (fileName.isEmpty())
         fileName = imposterPatch->getAppName() + ".py";
      scriptLabel->setText(fileName);
   }

   const QString error = process->getErrorBuffer();
   outputEdit->append(error);

   const QString output = process->getOutputBuffer();
   outputEdit->append(output);
}

void Python::Widget::slotChooseInterpreter()
{
#ifdef Q_OS_WINDOWS
   const QString filter = "python.exe";
#else
   const QString filter = "python3*";
#endif // Q_OS_WINDOWS

   QString executable = QFileDialog::getOpenFileName(this, "Python Executable", QString(), filter);
   if (executable.isEmpty())
      return;

   pythonExecutableEdit->setText(executable);

   Settings pythonSettings("python", true);
   pythonSettings.write("executable", executable);
}

void Python::Widget::slotSelectScript()
{
   QString fileName = QFileDialog::getOpenFileName(this, "Python Script", QString(), "*.py");
   if (fileName.isEmpty())
      return;

   scriptLabel->setText(fileName);

   Settings pythonSettings("python");
   pythonSettings.write("script", fileName);
}

void Python::Widget::slotProcessStarted()
{
   scriptButton->setChecked(true);
}

void Python::Widget::slotProcessAborted()
{
   scriptButton->setChecked(false);
   slotProcessError();
}

void Python::Widget::slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
   Q_UNUSED(exitCode)
   Q_UNUSED(exitStatus)

   scriptButton->setChecked(false);
}

void Python::Widget::slotProcessError()
{
   const QString error = process->getErrorBuffer();
   outputEdit->append(error);
}

void Python::Widget::slotProcessOutput()
{
   const QString output = process->getOutputBuffer();
   outputEdit->append(output);
}
