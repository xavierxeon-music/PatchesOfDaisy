#include "MainMesh.h"

#include "PageOverview.h"

MainMesh::MainMesh()
   : Abstract::Patch("MainMesh")
   , FlashSettings(this, 5)
   , grid(128)
   , table(128)
   , oscilator()
{
   load();

   pageManager.addPage(new PageOverview());

   auto slope = [&](const SoundMesh::Point& point) -> float
   {
      const float precent = static_cast<float>(point.xIndex) / static_cast<float>(grid.getSize() - 1);
      const float value = (2.0 * precent) - 1.0;

      return value;
   };
   grid.fill(slope);

   auto circle = [&](const float& angle) -> SoundMesh::Grid::PointF
   {
      const float x = 0.95 * sin(angle);
      const float y = 0.95 * cos(angle);

      return {x, y};
   };
   SoundMesh::Path path = grid.createPath(circle, table.getNoOfSteps());

   table.update(grid, path);
   oscilator.init(&table, daisy.AudioSampleRate());
}

void MainMesh::audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size)
{
   (void)in;

   const Input input = compileInput();
   const Encoder encoder = compileEncoder();
   updatePageManager(encoder);

   auto circle = [&](const float& angle) -> SoundMesh::Grid::PointF
   {
      float x = 0.2 * input.controlVoltages[1] * sin(angle);
      float y = 0.2 * input.controlVoltages[2] * cos(angle);

      const float angle2 = 0.2 * Maths::pi * input.controlVoltages[3];
      x = x * sin(angle2) + y * cos(angle2);
      y = x * cos(angle2) + y * sin(angle2);

      return {x, y};
   };
   SoundMesh::Path path = grid.createPath(circle, table.getNoOfSteps());
   table.update(grid, path);

   const float voltage = input.controlVoltages[0];
   const float frequency = Abstract::Oscilator::frequencyFromCV(voltage);
   oscilator.setFrequency(frequency);

   for (uint8_t channel = 0; channel < 4; channel++)
   {
      if (1 == channel)
         break;
      for (size_t chunk = 0; chunk < size; chunk++)
      {
         const float value = oscilator.createSound();
         out[channel][chunk] = value;
      }
   }
}

void MainMesh::nonAudioLoop()
{
   ensureSettignsValid();
}

// main function

int main(void)
{
   MainMesh app;
   app.run();
}
