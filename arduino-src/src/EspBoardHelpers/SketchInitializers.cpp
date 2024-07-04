#include <Arduino.h>
#include <Wire.h>
#include "SketchInitializers.h"
#include "PinDefinitions.h"

void SketchInitializers::InitializeSpiPins()
{
    constexpr const static uint8_t spi_host = 0;

    constexpr const static int8_t pin_cs = 6;

    constexpr const static int8_t pin_sclk = 8;

    constexpr const static int8_t pin_mosi = 5;

    constexpr const static int8_t pin_miso = 7;
}

void SketchInitializers::InitializeRemotePins()
{
    pinMode(PinDefinitions::PIN_UNLOCK, OUTPUT);
    digitalWrite(PinDefinitions::PIN_UNLOCK, LOW);

    pinMode(PinDefinitions::PIN_LOCK, OUTPUT);
    digitalWrite(PinDefinitions::PIN_LOCK, LOW);

    pinMode(PinDefinitions::PIN_ALARM, OUTPUT);
    digitalWrite(PinDefinitions::PIN_LOCK, LOW);
}