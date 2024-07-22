#include "CarHelper.h"

CarHelper::CarHelper(String *whichCarIn)
{
    whichCar = whichCarIn;
}

void CarHelper::UnlockAllDoors() const
{
    if (whichCar->equals("2008_HONDA"))
    {
        pinMode(Definitions::PIN_UNLOCK, OUTPUT);
        digitalWrite(Definitions::PIN_UNLOCK, LOW);

        delay(400);

        pinMode(Definitions::PIN_UNLOCK, INPUT);

        delay(400);

        pinMode(Definitions::PIN_UNLOCK, OUTPUT);
        digitalWrite(Definitions::PIN_UNLOCK, LOW);

        delay(400);

        pinMode(Definitions::PIN_UNLOCK, INPUT);
    }

}

void CarHelper::LockAllDoors() const
{
    if (whichCar->equals("2008_HONDA"))
    {
        pinMode(Definitions::PIN_LOCK, OUTPUT);
        digitalWrite(Definitions::PIN_LOCK, LOW);

        delay(400);

        pinMode(Definitions::PIN_LOCK, INPUT);
    }
}

void CarHelper::OpenTrunk() const
{
    if (whichCar->equals("2008_HONDA"))
    {
        pinMode(Definitions::PIN_TRUNK, OUTPUT);
        digitalWrite(Definitions::PIN_TRUNK, LOW);

        delay(1600);

        pinMode(Definitions::PIN_TRUNK, INPUT);

        Serial.println("Opened trunk!");
    }
}

// void CarHelper::Corolla2003UnlockAllDoors()
// {
// //    _espNeopixel->SetBuiltInLedRGB(255, 0, 0);
//
//     digitalWrite(Definitions::PIN_UNLOCK, HIGH);
//
//     delay(300);
//
//     digitalWrite(Definitions::PIN_UNLOCK, LOW);
//
//     delay(300);
//
//     digitalWrite(Definitions::PIN_UNLOCK, HIGH);
//
//     delay(300);
//
//     digitalWrite(Definitions::PIN_UNLOCK, LOW);
//
//     delay(500);
//
// //    _espNeopixel->SetBuiltInLedRGB(0, 0, 0);
// }

// void CarHelper::Lock()
// {
// //   _espNeopixel->SetBuiltInLedRGB(0, 255, 0);
//
//     digitalWrite(Definitions::PIN_LOCK, LOW);
//
//     delay(200);
//
//     digitalWrite(Definitions::PIN_LOCK, HIGH);
//
//     delay(500);
//
// //    _espNeopixel->SetBuiltInLedRGB(0, 0, 0);
// }
