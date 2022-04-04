#include <Midi/MidiDeviceInput.h>

#include <Midi/MidiInterfaceOutput.h>

Midi::Device::Input::Input(QObject* parent, const QString& inputPortName)
   : QObject(parent)
   , Interface::Input()
   , input()
   , inputPortName(inputPortName.toStdString())
{
}

Midi::Device::Input::~Input()
{
   Device::Input::close();
}

void Midi::Device::Input::open()
{
   openInput();
}

void Midi::Device::Input::close()
{
   if (!input.isPortOpen())
      return;

   input.closePort();
   qInfo() << "closed midi input port";
}

QStringList Midi::Device::Input::getAvailable()
{
   QStringList deviceList;

   RtMidiIn dummy;
   for (uint index = 0; index < dummy.getPortCount(); index++)
   {
      const std::string testPortName = dummy.getPortName(index);
      deviceList << QString::fromStdString(testPortName);
   }

   return deviceList;
}

void Midi::Device::Input::dataFromInput(const Bytes& message)
{
   for (Interface::Output* passthrough : passthroughList)
      passthrough->sendBuffer(message);

   Interface::Input::dataFromInput(message);
}

void Midi::Device::Input::openInput()
{
   if (input.isPortOpen())
      return;

   uint portNumber = 255;
   for (uint index = 0; index < input.getPortCount(); index++)
   {
      const std::string testPortName = input.getPortName(index);
      if (inputPortName != testPortName)
         continue;

      portNumber = index;
      break;
   }
   if (255 != portNumber)
   {
      input.openPort(portNumber);

      input.setErrorCallback(&Device::Input::midiError);
      input.setCallback(&Device::Input::midiReceive, this);
      input.ignoreTypes(false, false, false); // do not ignore anything

      qInfo() << "opened midi input port" << portNumber;
   }
   else
   {
      qWarning() << "unable to open midi input";
   }
}

void Midi::Device::Input::midiError(RtMidiError::Type type, const std::string& errorText, void* userData)
{
   Q_UNUSED(userData)

   qInfo() << "input" << type << QString::fromStdString(errorText);
}

void Midi::Device::Input::midiReceive(double timeStamp, std::vector<unsigned char>* message, void* userData)
{
   Q_UNUSED(timeStamp)

   if (!message || !userData)
      return;

   Input* me = reinterpret_cast<Input*>(userData);
   if (!me)
      return;

   static int once = qRegisterMetaType<Bytes>("Bytes");
   Q_UNUSED(once);

   static Bytes buffer;
   auto maybeProcessBuffer = [&]()
   {
      if (0 == buffer.size())
         return;

      Input::staticMetaObject.invokeMethod(me, "dataFromInput", Qt::QueuedConnection, Q_ARG(Bytes, buffer));

      //me->dataFromInput(buffer); // may cause threading issues, since callback is not in main thread

      buffer.clear();
   };

   static const uint8_t mask = 0x80;
   for (const uint8_t byte : *message)
   {
      const uint8_t test = byte & mask;
      if (test == mask) // new message start
         maybeProcessBuffer();

      buffer << byte;
   }
   maybeProcessBuffer();
}
