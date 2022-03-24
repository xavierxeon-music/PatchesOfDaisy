#!/bin/bash

# path variable
echo "Installing DaisyToolchain"

# install brew
if ! command -v brew &> /dev/null
then
    echo "Installing Homebrew: Follow onscreen instructions"
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
fi

#upgrade homebrew
echo "Upgrading Homebrew"
brew update
brew upgrade 

echo "Installing packages with Homebrew"
brew install open-ocd dfu-util gcc-arm-embedded

if [ -d /opt/homebrew/Caskroom ]
then
   CASKROOM=/opt/homebrew/Caskroom
else
   CASKROOM=/usr/local/Caskroom
fi

find $CASKROOM/gcc-arm-embedded -type f -perm +111 -print | xargs spctl --add --label "gcc-arm-embedded"
find $CASKROOM/gcc-arm-embedded | xargs xattr -d com.apple.quarantine
