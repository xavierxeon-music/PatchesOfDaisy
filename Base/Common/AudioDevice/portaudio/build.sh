#!/bin/bash

PORTAUDIO_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"  

if [ -d $PORTAUDIO_DIR/install ]
then
   exit
fi

mkdir $PORTAUDIO_DIR/build

cd $PORTAUDIO_DIR
git submodule update --init --recursive

cd $PORTAUDIO_DIR/build
$PORTAUDIO_DIR/source/configure --disable-mac-universal --prefix=$PORTAUDIO_DIR/install

make
make install
