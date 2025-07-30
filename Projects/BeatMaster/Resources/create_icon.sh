#!/bin/bash

ICON_LIST=$(ls -1 icon_*.txt)

OUTFILE=../IconsData.cpp

echo "#include \"Icons.h\"" > $OUTFILE
echo "// clang-format off" >> $OUTFILE

for ICON_FILE in $ICON_LIST
do
   echo $ICON_FILE
   ../../../Base/scripts/bitmask_from_ascii.py $ICON_FILE >> $OUTFILE
done

echo "// clang-format on" >> $OUTFILE
