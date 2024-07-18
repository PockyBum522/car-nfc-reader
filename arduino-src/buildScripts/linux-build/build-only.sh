#!/bin/bash

cd ../..

pio run

cd .pio\build\esp32-s3-devkitc-1\

nemo . &

echo "Drag firmware.bin onto the ElegantOTA page"

firefox "http://192.168.1.111/update"

cd ../../../buildScripts/linux-build
