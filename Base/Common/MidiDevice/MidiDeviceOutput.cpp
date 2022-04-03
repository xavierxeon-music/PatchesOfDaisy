#include <Midi/MidiDeviceOutput.h>

Midi::Device::Output::Output(const QString& outputPortName)
   : Interface::Output()
   , output()
   , outputPortName(outputPortName.toStdString())
{
}

Midi::Device::Output::~Output()
{
   if (output.isPortOpen())
   {
      output.closePort();
      qInfo() << "closed midi output port";
   }
}

void Midi::Device::Output::initMidi(bool verbose)
{
   openOutput(verbose);
}

void Midi::Device::Output::sendBuffer(const Bytes& buffer)
{
   output.sendMessage(&buffer);
}

void Midi::Device::Output::openOutput(bool verbose)
{
   if (output.isPortOpen())
      return;

   if (verbose)
      qInfo() << "available outputs:";

   uint portNumber = 255;
   for (uint index = 0; index < output.getPortCount(); index++)
   {
      const std::string testPortName = output.getPortName(index);
      if (verbose)
         qDebug() << index << QString::fromStdString(testPortName);

      if (outputPortName != testPortName)
         continue;

      portNumber = index;
      break;
   }

   if (255 != portNumber)
   {
      output.openPort(portNumber);
      output.setErrorCallback(&Midi::Device::Output::midiError);

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
