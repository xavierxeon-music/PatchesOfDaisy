#include "MainWidget.h"

#include <QApplication>
#include <QFileDialog>

#include <QThread>

MainWidget::MainWidget(QWidget* parent)
   : QWidget(parent)
   , driver()
   , mutex()
   , revoiceLeft()
   , revoiceRight()
   , meta()
   , playData()
   , playhead(0)
{
   driver.registerCallbackOverrideFunction(this, &MainWidget::audioCallback);
   setupUi(this);

   revoiceLeft.setSampleRate(driver.getSampleRate());
   revoiceRight.setSampleRate(driver.getSampleRate());

   connect(loadButton, &QAbstractButton::clicked, this, &MainWidget::slotLoad);
   connect(saveButton, &QAbstractButton::clicked, this, &MainWidget::slotSave);
}

void MainWidget::slotLoad()
{
   //const QString fileName = QFileDialog::getOpenFileName(this, "WAV file", QString(), "*.wav");
   const QString fileName = "/Users/waspe/Downloads/AMG 260db/Track 01.wav";
   if (fileName.isEmpty())
      return;

   mutex.lock();
   playhead = 0;
   playData = AudioFile::load(fileName.toStdString(), &meta);
   mutex.unlock();
}

void MainWidget::slotSave()
{
   //const QString fileName = QFileDialog::getSaveFileName(this, "WAV file", QString(), "*.wav");
   const QString fileName = "/Users/waspe/Desktop/Test.wav";
   if (fileName.isEmpty())
      return;

   mutex.lock();
   const Data inData = playData;
   const AudioFile::Meta outMeta = meta;
   mutex.unlock();

   Revoice outRevoice;
   outRevoice.setSampleRate(driver.getSampleRate());

   Data outData(inData.size(), 0.0);

   static const uint16_t bufferSize = Spectrum::compileBufferSize();
   if (!outMeta.stereo)
   {
      for (size_t offset = 0; offset < inData.size(); offset += bufferSize)
      {
         Data inBuffer(bufferSize);
         for (uint16_t index = 0; index < bufferSize; index++)
         {
            const size_t fullIndex = offset + index;
            inBuffer[index] = fullIndex < inData.size() ? inData[fullIndex] : 0.0;
         }

         Data outBuffer = outRevoice.convert(inBuffer);
         for (uint16_t index = 0; index < bufferSize; index++)
         {
            const size_t fullIndex = offset + index;
            if (fullIndex < inData.size())
               outData[fullIndex] = outBuffer[index];
         }
      }
   }
   else
   {
      for (size_t offset = 0; offset < inData.size(); offset += 2 * bufferSize)
      {
         Data inBufferLeft(bufferSize);
         Data inBufferRight(bufferSize);
         for (uint16_t index = 0; index < bufferSize; index++)
         {
            const size_t fullIndexLeft = offset + (2 * index) + 0;
            inBufferLeft[index] = fullIndexLeft < inData.size() ? inData[fullIndexLeft] : 0.0;

            const size_t fullIndexRight = fullIndexLeft + 1;
            inBufferRight[index] = fullIndexRight < inData.size() ? inData[fullIndexRight] : 0.0;
         }

         const Data outBufferLeft = outRevoice.convert(inBufferLeft);
         const Data outBufferRight = outRevoice.convert(inBufferRight);

         for (uint16_t index = 0; index < bufferSize; index++)
         {
            const size_t fullIndexLeft = offset + (2 * index) + 0;
            if (fullIndexLeft < inData.size())
               outData[fullIndexLeft] = outBufferLeft[index];

            const size_t fullIndexRight = fullIndexLeft + 1;
            if (fullIndexRight < inData.size())
               outData[fullIndexRight] = outBufferRight[index];
         }
      }
   }

   AudioFile::save(fileName.toStdString(), outMeta, outData);
}

void MainWidget::audioCallback(const void* inputBuffer, void* outputBuffer, const AudioDevice::Frame& framesPerBuffer)
{
   Q_UNUSED(inputBuffer)

   auto advancePlayhead = [&]()
   {
      playhead++;
      if (playData.size() <= playhead)
         playhead = 0;
   };

   float* output = static_cast<float*>(outputBuffer);

   mutex.lock();
   for (AudioDevice::Frame frame = 0; frame < framesPerBuffer; frame++)
   {
      const size_t indexLeft = 0 + (frame * driver.getMaxOutputChannels());
      const size_t indexRight = 1 + (frame * driver.getMaxOutputChannels());

      if (0 == meta.numberOfSamples)
      {
         output[indexLeft] = 0;
         output[indexRight] = 0;
      }
      else
      {
         const float inValueLeft = playData.at(playhead);
         advancePlayhead();

         const float outValueLeft = revoiceLeft.changeSound(inValueLeft);
         output[indexLeft] = outValueLeft;

         if (meta.stereo)
         {
            const float inValueRight = playData.at(playhead);
            advancePlayhead();

            const float outValueRight = revoiceRight.changeSound(inValueRight);
            output[indexRight] = outValueRight;
         }
         else
         {
            output[indexRight] = outValueLeft;
         }
      }
   }
   mutex.unlock();
}

// main function
int main(int argc, char** argv)
{
   QApplication::setApplicationName("HarmonizerUI");
   QApplication::setOrganizationDomain("eu.schweinesystem");

   QApplication app(argc, argv);

   MainWidget mw(nullptr);
   mw.show();

   return app.exec();
}
