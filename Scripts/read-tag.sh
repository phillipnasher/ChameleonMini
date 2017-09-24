#!/bin/bash


# Script for reading a tag from the Chameleon mini device.
# Refer to http://rawgit.com/emsec/ChameleonMini/master/Doc/Doxygen/html/Page_CommandLine.html for a list of commands 
# 
#

echo "setting=2" | socat - /dev/ttyACM1,crnl
echo "config=none" | socat - /dev/ttyACM1,crnl
echo "config=ISO14443A_READER" | socat - /dev/ttyACM1,crnl
echo "identify" | socat - /dev/ttyACM1,crnl
echo "dump_mfu" | socat - /dev/ttyACM1,crnl
