#!/bin/bash

cd "$(brew --repo homebrew/core)"
#git log master -- Formula/libusb.rb
git checkout 49987f2c44f6686c9bf104a6f27c1b227aceab7b # version 1.0.24 of libusb
HOMEBREW_NO_AUTO_UPDATE=1 brew reinstall libusb
brew pin libusb
git checkout master

