#include "MainWindow.h"

#include <QAction>
#include <QCloseEvent>
#include <QDockWidget>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QSpinBox>
#include <QToolBar>
#include <QVBoxLayout>

#include "HardwareWidget.h"
#include "ImposterDaisyPatch.h"
#include "LogWidget.h"
#include "PythonWidget.h"
#include "Settings.h"
#include "TempoWidget.h"

MainWindow::MainWindow(Imposter::DaisyPatch* imposterPatch)
   : QMainWindow(nullptr)
   , imposterPatch(imposterPatch)
   , hardwwareWidget(nullptr)
   , server(nullptr)
{
   setWindowTitle("IMPOSTER DAISY PATCH - " + imposterPatch->getAppName());
   setWindowIcon(QIcon(":/Imposter.svg"));

   server = new Python::Server(this);

   createControlWidget();

   hardwwareWidget = new HardwareWidget(this, imposterPatch);
   setCentralWidget(hardwwareWidget);

   Settings windowSettings("window", true);
   restoreGeometry(windowSettings.bytes("geometry"));
   restoreState(windowSettings.bytes("state"));
}

void MainWindow::updateState()
{
   hardwwareWidget->updateState();
}

void MainWindow::externalControls()
{
   clientInput();
   emit signalSetExternalControls();
}

void MainWindow::applyOutput(const Output& output)
{
   // TODO send to python
   hardwwareWidget->applyOutput(output);
}

void MainWindow::clientInput()
{
   static const QString inputTag = "SetInput";

   auto resetInputs = [&]()
   {
      imposterPatch->controls[0].setRemote(0.0);
      imposterPatch->controls[1].setRemote(0.0);
      imposterPatch->controls[2].setRemote(0.0);
      imposterPatch->controls[3].setRemote(0.0);

      imposterPatch->gate_input[0].activateRemote(false);
      imposterPatch->gate_input[1].activateRemote(false);
   };

   QJsonObject inputRequest;
   inputRequest["request"] = inputTag;
   inputRequest["audioCallbackRate"] = imposterPatch->AudioCallbackRate();

   QJsonObject inputReply = server->sendToClient(inputRequest);
   if (!inputReply.contains("reply") || !inputReply.contains("data"))
      return resetInputs();

   const QString replyTag = inputReply["reply"].toString();
   if (inputTag != replyTag)
      return resetInputs();

   const QJsonObject& dataObject = inputReply["data"].toObject();
   //qDebug() << dataObject;

   const QJsonArray& cvArray = dataObject["cv"].toArray();
   for (uint8_t cvIndex = 0; cvIndex < cvArray.count(); cvIndex++)
   {
      const QJsonValue& value = cvArray.at(cvIndex);
      const float cv = value.toDouble();
      imposterPatch->controls[cvIndex].setRemote(cv);
   }

   const QJsonArray& gateArray = dataObject["gate"].toArray();
   for (uint8_t gateIndex = 0; gateIndex < gateArray.count(); gateIndex++)
   {
      const QJsonValue& value = gateArray.at(gateIndex);
      const bool gate = value.toBool();
      imposterPatch->gate_input[gateIndex].activateRemote(gate);
   }

   QJsonArray waveArray = dataObject["wave"].toArray();
   for (uint8_t waveIndex = 0; waveIndex < waveArray.count(); waveIndex++)
   {
      const QJsonObject& waveObject = waveArray.at(waveIndex).toObject();
      const int note = waveObject["note"].toInt();
      const int shape = waveObject["shape"].toInt();

      AudioChannel::Input& input = imposterPatch->audioHandle.inChannel[waveIndex];
      input.setRemote(Note::fromMidi(note), static_cast<Standard::Waveform::Shape>(shape));
   }
}

void MainWindow::closeEvent(QCloseEvent* ce)
{
   Settings windowSettings("window", true);
   windowSettings.write("geometry", saveGeometry());
   windowSettings.write("state", saveState());

   ce->accept();
}

void MainWindow::createControlWidget()
{
   QWidget* controlWidget = new QWidget(this);

   TempoWidget* tempoWidget = new TempoWidget(controlWidget, imposterPatch);
   Python::Widget* pythonWidget = new Python::Widget(controlWidget, imposterPatch);
   LogWidget* logWidget = new LogWidget(this);

   connect(this, &MainWindow::signalSetExternalControls, tempoWidget, &TempoWidget::slotSetExternalControls);

   auto line = [&]()
   {
      QFrame* hl = new QFrame(controlWidget);
      hl->setFrameShape(QFrame::HLine);
      hl->setFrameShadow(QFrame::Plain);
      return hl;
   };

   QVBoxLayout* masterLayout = new QVBoxLayout(controlWidget);
   masterLayout->setContentsMargins(2, 2, 2, 2);
   masterLayout->setSpacing(0);
   masterLayout->addWidget(tempoWidget);
   masterLayout->addWidget(line());
   masterLayout->addWidget(logWidget);
   masterLayout->addWidget(line());
   masterLayout->addWidget(pythonWidget);

   QDockWidget* controlDock = new QDockWidget(this);
   controlDock->setObjectName("ControlDock");
   controlDock->setWidget(controlWidget);
   addDockWidget(Qt::LeftDockWidgetArea, controlDock);
   controlDock->setTitleBarWidget(new QWidget());
   controlDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
}
