#!/bin/bash

#Script needs to be executed with sudo rights

#FIRMWARE_DIR="../Code/ChameleonMini/Firmware/Chameleon-Mini/"
FIRMWARE_DIR="../Firmware/Chameleon-Mini/"


( cd ${FIRMWARE_DIR} ; make clean )
( cd ${FIRMWARE_DIR} ; make )


avrdude -c flip2 -p ATXMega128A4U -B 60 -P usb -U application:w:${FIRMWARE_DIR}Chameleon-Mini.hex:i -U eeprom:w:${FIRMWARE_DIR}Chameleon-Mini.eep:i
