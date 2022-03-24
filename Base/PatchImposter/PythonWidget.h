#ifndef PythonWidgetH
#define PythonWidgetH

#include "ui_PythonWidget.h"
#include <QWidget>

#include "PythonProcess.h"

namespace Imposter
{
   class DaisyPatch;
}

namespace Python
{
   class Widget : public QWidget, private Ui::Widget
   {
      Q_OBJECT
   public:
      Widget(QWidget* parent, Imposter::DaisyPatch* imposterPatch);

   private slots:
      // buttons
      void slotChooseInterpreter();
      void slotSelectScript();
      // attach to process
      void slotProcessStarted();
      void slotProcessAborted();
      void slotProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);
      void slotProcessError();
      void slotProcessOutput();

   private:
      Process* process;
   };
} // namespace Python

#endif // PythonWidgetH
