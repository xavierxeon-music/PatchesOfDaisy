@echo off
setlocal

set PORTAUDIO_DIR=%~dp0
if exist %PORTAUDIO_DIR%install exit

call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64

cd %PORTAUDIO_DIR%
git submodule update --init --recursive

mkdir build
cd build

cmake %PORTAUDIO_DIR%source -DCMAKE_GENERATOR:STRING=Ninja -DBUILD_TESTS=OFF -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=%PORTAUDIO_DIR%install

ninja 
ninja install 

cd %PORTAUDIO_DIR%
