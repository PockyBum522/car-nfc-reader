#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H


#include <Arduino.h>
#include "../Models/Definitions.h"
#include "Models/WhichCar.h"

class CarHelper
{
public:
    explicit CarHelper(const WhichCar whichCarIn)
    {
        m_whichCar = whichCarIn;
    }

    void UnlockAllDoors() const
    {
        if (m_whichCar == WhichCar::HONDA_2008)
        {
            pinMode(Definitions::PIN_UNLOCK, OUTPUT);
            digitalWrite(Definitions::PIN_UNLOCK, LOW);

            delay(200);

            pinMode(Definitions::PIN_UNLOCK, INPUT);

            delay(200);

            pinMode(Definitions::PIN_UNLOCK, OUTPUT);
            digitalWrite(Definitions::PIN_UNLOCK, LOW);

            delay(200);

            pinMode(Definitions::PIN_UNLOCK, INPUT);
        }

        // Pins are screwy on COROLLA_2021
        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            pinMode(Definitions::PIN_LOCK, OUTPUT);
            digitalWrite(Definitions::PIN_LOCK, LOW);

            delay(400);

            pinMode(Definitions::PIN_LOCK, INPUT);

            delay(400);

            pinMode(Definitions::PIN_LOCK, OUTPUT);
            digitalWrite(Definitions::PIN_LOCK, LOW);

            delay(400);

            pinMode(Definitions::PIN_LOCK, INPUT);
        }
    }

    void LockAllDoors() const
    {
        if (m_whichCar == WhichCar::HONDA_2008)
        {
            pinMode(Definitions::PIN_LOCK, OUTPUT);
            digitalWrite(Definitions::PIN_LOCK, LOW);

            delay(200);

            pinMode(Definitions::PIN_LOCK, INPUT);
        }

        // Pins are screwy on this one
        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            pinMode(Definitions::PIN_UNLOCK, OUTPUT);
            digitalWrite(Definitions::PIN_UNLOCK, LOW);

            delay(400);

            pinMode(Definitions::PIN_UNLOCK, INPUT);
        }
    }

    void OpenTrunk() const
    {
        if (m_whichCar == WhichCar::HONDA_2008)
        {
            delay(500);

            pinMode(Definitions::PIN_TRUNK, OUTPUT);
            digitalWrite(Definitions::PIN_TRUNK, LOW);

            delay(500);

            pinMode(Definitions::PIN_TRUNK, INPUT);

            Serial.println("Opened trunk!");
        }

        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            delay(500);

            pinMode(Definitions::PIN_TRUNK, OUTPUT);
            digitalWrite(Definitions::PIN_TRUNK, LOW);

            delay(500);

            pinMode(Definitions::PIN_TRUNK, INPUT);

            Serial.println("Opened trunk!");
        }
    }

    static std::string WhichCarToString(const WhichCar whichCar)
    {
        if (whichCar == WhichCar::HONDA_2008)
            return "HONDA_2008";

        if (whichCar == WhichCar::COROLLA_2003)
            return "COROLLA_2003";

        if (whichCar == WhichCar::COROLLA_2021)
            return "COROLLA_2021";

        return "ERROR";
    }

private:
    WhichCar m_whichCar;
};


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
