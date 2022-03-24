#!/bin/bash

cd "$(brew --repo homebrew/cask)"
#git log master -- Casks/gcc-arm-embedded.rb
git checkout d407663b8017a0a062c7fc0b929faf2e16abd1ff # version 10.3 of gcc-arm-embedded
HOMEBREW_NO_AUTO_UPDATE=1 brew reinstall gcc-arm-embedded
git checkout master
