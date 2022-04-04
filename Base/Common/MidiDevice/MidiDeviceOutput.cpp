#include <Midi/MidiDeviceOutput.h>

#include <QThread>

Midi::Device::Output::Output(QObject* parent, const QString& outputPortName)
   : QObject(parent)
   , Interface::Output()
   , output()
   , outputPortName(outputPortName.toStdString())
{
}

Midi::Device::Output::~Output()
{
   Device::Output::close();
}

void Midi::Device::Output::open()
{
   openOutput();
}

void Midi::Device::Output::close()
{
   if (!output.isPortOpen())
      return;

   output.closePort();
   qInfo() << "closed midi output port";
}

QStringList Midi::Device::Output::getAvailable()
{
   QStringList deviceList;

   RtMidiOut dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      deviceList << QString::fromStdString(testPortName);
   }

   return deviceList;
}

void Midi::Device::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
   QThread::msleep(1);
}

void Midi::Device::Output::openOutput()
{
   if (output.isPortOpen())
      return;

   uint portNumber = 255;
   for (uint index = 0; index < output.getPortCount(); index++)
   {
      const std::string testPortName = output.getPortName(index);
      if (outputPortName != testPortName)
         continue;

      portNumber = index;
      break;
   }

   if (255 != portNumber)
   {
      output.openPort(portNumber);
      output.setErrorCallback(&Device::Output::midiError);

      qInfo() << "opened midi output port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi output";
   }
}

void Midi::Device::Output::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "output" << type << QString::fromStdString(errorText);
}
