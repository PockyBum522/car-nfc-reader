cd ../..

pio run

cd .pio\build\esp32-s3-devkitc-1\

explorer .

echo "Drag firmware.bin onto the ElegantOTA page"

explorer "http://192.168.1.110/update"

cd ../../../buildScripts/windowsBuild
