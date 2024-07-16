#include "CarHelper.h"

CarHelper::CarHelper(String *whichCarIn)
{
    whichCar = whichCarIn;
}

void CarHelper::UnlockAllDoors() const
{
    if (whichCar->equals("CAR_IS_2008_HONDA"))
    {
        pinMode(Definitions::PIN_UNLOCK, OUTPUT);
        digitalWrite(Definitions::PIN_UNLOCK, LOW);

        delay(500);

        pinMode(Definitions::PIN_UNLOCK, INPUT);

        delay(500);

        pinMode(Definitions::PIN_UNLOCK, OUTPUT);
        digitalWrite(Definitions::PIN_UNLOCK, LOW);

        delay(500);

        pinMode(Definitions::PIN_UNLOCK, INPUT);

        delay(500);
    }

}

void CarHelper::LockAllDoors() const
{
    if (whichCar->equals("CAR_IS_2008_HONDA"))
    {
        pinMode(Definitions::PIN_LOCK, OUTPUT);
        digitalWrite(Definitions::PIN_LOCK, LOW);

        delay(500);

        pinMode(Definitions::PIN_LOCK, INPUT);

        delay(500);
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
