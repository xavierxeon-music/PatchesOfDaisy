#!/bin/bash

VERSION=v4.0.0

START_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null && pwd )"   
LIBDAISY_DIR=$START_DIR/libDaisy

echo "building libDaisy . . ."
cd $LIBDAISY_DIR 
make clean

BRANCH_EXIST_TEST=$(git branch -l | grep $VERSION)
if [ -z "$BRANCH_EXIST_TEST" ]
then
   git checkout tags/$VERSION -b $VERSION
fi

BRANCH_CURRENT_TEST=$(git branch --show-current)
if [ "$BRANCH_CURRENT_TEST" != "$VERSION" ]
then
   git switch $VERSION 
fi

make | grep "warning:r\|error" ;
echo "done."

git switch master
