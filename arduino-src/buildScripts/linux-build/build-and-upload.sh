##!/bin/bash
#
#PROJECT_BASE_PATH="/home/david/source/repos/kit-contracting-work/non-contracted work/esp32-s3-small-rotovap-controller"
#
#cd "$PROJECT_BASE_PATH" || return
#
#BUILD_BASE_PATH="$PROJECT_BASE_PATH/.pio/build/esp32-s3-devkitc-1"
#
##pio run
#
##FIRMWARE_PATH="$BUILD_BASE_PATH/firmware.bin"
#FIRMWARE_PATH="$BUILD_BASE_PATH/firmware.bin.06old"
#
#HOST_BASE_URL="http://192.168.1.60"
#HOST_BUILT_OTA_URL="$HOST_BASE_URL/ota"
#
## Get and trim MD5 hash
#HASH_RAW=$(md5sum "$FIRMWARE_PATH")
#IFS=' ' read -a SUM_COMMAND_ARRAY <<< "$HASH_RAW"
#HASH_ONLY="${SUM_COMMAND_ARRAY[0]}"
#
#echo ""
#echo "For project in:"
#echo "$PROJECT_BASE_PATH"
#echo ""
#echo "Built:"
#echo "$FIRMWARE_PATH"
#echo ""
#echo "Raw hash:"
#
#echo "$HASH_RAW"
#echo ""
#echo "Trimmed hash:"
#echo "$HASH_ONLY"
#echo ""
#echo "In:"
#echo "$BUILD_BASE_PATH"
#echo ""
##echo "Uploading built file to:"
##echo "$HOST_BUILT_FULL_URL"
#echo ""
#
#FULL_START_CURL="$HOST_BUILT_OTA_URL/start?mode=fr&hash=$HASH_ONLY"
#
##echo "Invoking FW upload start URL:"
##echo "$FULL_START_CURL"
#echo ""
#
## curl "$FULL_START_CURL"
#
#echo ""
#
#curl 'http://192.168.1.60/update'
#
#sleep 2
#
#curl 'http://192.168.1.60/ota/start?mode=fr&hash=441018525208457705bf09a8ee3c1093'
#
#sleep 2
#
##curl 'http://192.168.1.60/ota/upload' -X POST -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate' -H 'Origin: http://192.168.1.60' -H 'Connection: keep-alive' -H 'Referer: http://192.168.1.60/update' --data-binary "-----------------------------212025887927634863191982760536\r\nContent-Disposition: form-data; name=\"file\"; filename=\"firmware.bin\"\r\nContent-Type: application/octet-stream\r\n\r\n@$FIRMWARE_PATH-----------------------------212025887927634863191982760536--"
#
## Empty response
##curl 'http://192.168.1.60/ota/upload?name=file&filename=firmware.bin' -X POST -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate' -H 'Content-Type: multipart/form-data' -H 'Origin: http://192.168.1.60' -H 'Connection: keep-alive' -H 'Referer: http://192.168.1.60/update' -H "Content-Disposition: form-data" -H "Content-Type: application/octet-stream" -H "boundary=---------------------------21836024463898801466427190243" -d @"$FIRMWARE_PATH" -H "-----------------------------212025887927634863191982760536--"
#
## Says ok, doesn't actually do new upload
##curl 'http://192.168.1.60/ota/upload' -X POST -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate' -H 'Origin: http://192.168.1.60' -H 'Connection: keep-alive' -H 'Referer: http://192.168.1.60/update' --data-binary "-----------------------------212025887927634863191982760536\r\nContent-Disposition: form-data; name=\"file\"; filename=\"firmware.bin\"\r\nContent-Type: application/octet-stream\r\n\r\n@$FIRMWARE_PATH\r\n-----------------------------212025887927634863191982760536--\r\n"
#
#curl 'http://192.168.1.60/ota/upload' -X POST -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate' -H 'Content-Type: multipart/form-data; boundary=---------------------------21836024463898801466427190243' -H 'Origin: http://192.168.1.60' -H 'Connection: keep-alive' -H 'Referer: http://192.168.1.60/update' --data-binary "-----------------------------212025887927634863191982760536\r\nContent-Disposition: form-data; name=\"file\"; filename=\"firmware.bin\"\r\nContent-Type: application/octet-stream\r\n\r\n@/home/david/Desktop/firmware.bin\r\n-----------------------------212025887927634863191982760536--\r\n"
#
#
#echo
#
#
## Sort of, does whatever file was last uploaded through the GUI webpage
##curl 'http://192.168.1.60/ota/upload' -X POST -H 'User-Agent: Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0' -H 'Accept: */*' -H 'Accept-Language: en-US,en;q=0.5' -H 'Accept-Encoding: gzip, deflate' -H 'Origin: http://192.168.1.60' -H 'Connection: keep-alive' -H 'Referer: http://192.168.1.60/update' --data-binary "-----------------------------212025887927634863191982760536\r\nContent-Disposition: form-data; name=\"file\"; filename=\"firmware.bin\"\r\nContent-Type: application/octet-stream\r\n\r\n-----------------------------212025887927634863191982760536--\r\n@$FIRMWARE_PATH\r\n-----------------------------212025887927634863191982760536--"
#
#echo ""
