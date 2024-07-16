#!/bin/bash

PROJECT_BASE_PATH="/media/secondary/repos/kit-contracting-work/non-contracted work/esp32-s3-small-rotovap-controller"

cd "$PROJECT_BASE_PATH" || return

BUILD_BASE_PATH="$PROJECT_BASE_PATH/.pio/build/esp32-s3-devkitc-1"

FIRMWARE_PATH="$BUILD_BASE_PATH/firmware.bin"

echo ""
echo "For project in:"
echo "$PROJECT_BASE_PATH"
echo ""
echo "Built:"
echo "$FIRMWARE_PATH"
echo ""
echo "In:"
echo "$BUILD_BASE_PATH"
echo ""

pio run

echo ""
