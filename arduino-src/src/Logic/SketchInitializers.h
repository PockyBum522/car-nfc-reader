#ifndef COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
#define COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H


#include "Models/Definitions.h"
#include "Models/WhichCar.h"

class SketchInitializers
{
public:
    static void InitializeSpiPins()
    {
        constexpr const static uint8_t spi_host = 0;

        constexpr const static int8_t pin_cs = 6;

        constexpr const static int8_t pin_sclk = 8;

        constexpr const static int8_t pin_mosi = 5;

        constexpr const static int8_t pin_miso = 7;
    }

    static void InitializeRemotePins(const WhichCar whichCar)
    {
        if (whichCar == WhichCar::HONDA_2008)
        {
            pinMode(Definitions::PIN_UNLOCK, INPUT);
            pinMode(Definitions::PIN_LOCK, INPUT);
            pinMode(Definitions::PIN_TRUNK, INPUT);
        }

        if (whichCar == WhichCar::COROLLA_2003)
        {
            pinMode(Definitions::PIN_UNLOCK, OUTPUT);
            digitalWrite(Definitions::PIN_UNLOCK, LOW);

            pinMode(Definitions::PIN_LOCK, OUTPUT);
            digitalWrite(Definitions::PIN_LOCK, LOW);

            // This is actually the alarm pin, I think
            pinMode(Definitions::PIN_TRUNK, OUTPUT);
            digitalWrite(Definitions::PIN_LOCK, LOW);
        }

        if (whichCar == WhichCar::COROLLA_2021)
        {
            Definitions::PIN_UNLOCK = 42;
            Definitions::PIN_LOCK = 41;
            Definitions::PIN_TRUNK = 40;

            pinMode(Definitions::PIN_UNLOCK, INPUT);
            pinMode(Definitions::PIN_LOCK, INPUT);
            pinMode(Definitions::PIN_TRUNK, INPUT);
        }
    }
};


#endif //COROLLA_2003_NFC_CONTROLLER_SKETCHINITIALIZERS_H
