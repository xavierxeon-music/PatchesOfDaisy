#ifndef MainWindowH
#define MainWindowH

#include <QMainWindow>

#include "PythonServer.h"

#include <Abstract/Abstract.h>

namespace Imposter
{
   class DaisyPatch;
}

class HardwareWidget;

class MainWindow : public QMainWindow
{
   Q_OBJECT
public:
   MainWindow(Imposter::DaisyPatch* imposterPatch);

signals:
   void signalSetExternalControls();

public:
   void updateState();
   void externalControls();
   void applyOutput(const Output& output);

private:
   void clientInput();
   void closeEvent(QCloseEvent* ce) override;
   void createControlWidget();

private:
   Imposter::DaisyPatch* imposterPatch;
   HardwareWidget* hardwwareWidget;
   Python::Server* server;
};

#endif // MainWindowH
