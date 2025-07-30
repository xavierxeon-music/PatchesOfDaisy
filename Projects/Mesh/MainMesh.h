#ifndef MainMeshH
#define MainMeshH

#include <Abstract/AbstractPatch.h>
#include <Storage/FlashSettings.h>

#include <Remember.h>
#include <Sound/SoundMesh.h>

class MainMesh : public Abstract::Patch, public FlashSettings
{
public:
   MainMesh();

private:
   void audioLoop(AudioHandle::InputBuffer in, AudioHandle::OutputBuffer out, size_t size) override;
   void nonAudioLoop() override;

private: // things to remember

private:
   SoundMesh::Grid grid;
   SoundMesh::Table table;
   WaveTable::Oscilator oscilator;
};

#endif // MainMeshH
