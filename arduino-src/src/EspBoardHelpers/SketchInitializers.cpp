#include <Arduino.h>
#include <Wire.h>
#include "SketchInitializers.h"
#include "Definitions.h"

void SketchInitializers::InitializeSpiPins()
{
    constexpr const static uint8_t spi_host = 0;

    constexpr const static int8_t pin_cs = 6;

    constexpr const static int8_t pin_sclk = 8;

    constexpr const static int8_t pin_mosi = 5;

    constexpr const static int8_t pin_miso = 7;
}

void SketchInitializers::InitializeRemotePins(const String &whichCar)
{
    if (whichCar == String("2008_HONDA"))
    {
        pinMode(Definitions::PIN_UNLOCK, INPUT);
        pinMode(Definitions::PIN_LOCK, INPUT);
        pinMode(Definitions::PIN_TRUNK, INPUT);
    }

    if (whichCar == String("2003_COROLLA"))
    {
        pinMode(Definitions::PIN_UNLOCK, OUTPUT);
        digitalWrite(Definitions::PIN_UNLOCK, LOW);

        pinMode(Definitions::PIN_LOCK, OUTPUT);
        digitalWrite(Definitions::PIN_LOCK, LOW);

        // This is actually the alarm pin, I think
        pinMode(Definitions::PIN_TRUNK, OUTPUT);
        digitalWrite(Definitions::PIN_LOCK, LOW);
    }

    if (whichCar == String("2021_COROLLA"))
    {
        pinMode(Definitions::PIN_UNLOCK, INPUT);
        pinMode(Definitions::PIN_LOCK, INPUT);
        pinMode(Definitions::PIN_TRUNK, INPUT);
    }
}

