#!/bin/bash

function print_help_and_exit {
   echo "firmware GET_DFU path_to_bin_file"
   echo "   or"
   echo "firmware SET_DFU path_to_bin_file"
   echo "   or"
   echo "firmware FLASH path_to_bin_file"
   exit
}

MODE=$1
if [ -z "$MODE" ]
then
   print_help_and_exit
fi

FILE=$2
if [ -z "$FILE" ]
then
   print_help_and_exit
fi

if [ "$MODE" == "GET_DFU" ]
then
   if [ -f $FILE ]
   then
      rm $FILE
   fi
   echo "download from devive"
   dfu-util -a 0 -s 0x08000000:131072 -U $FILE -d ,0483:df11 # get 128kb
elif [ "$MODE" == "SET_DFU" ]
then
   echo "upload to devive"
   dfu-util -a 0 -s 0x08000000:leave -D $FILE -d ,0483:df11
elif [ "$MODE" == "FLASH" ]
then
   echo "use debugger to flash device"
   openocd -s /usr/local/share/openocd/scripts -f interface/stlink.cfg -f target/stm32h7x.cfg -c "program $FILE reset exit 0x08000000"
else
   print_help_and_exit
fi

