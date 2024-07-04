#include "CarHelper.h"

void CarHelper::UnlockDriverDoor()
{
//    _espNeopixel->SetBuiltInLedRGB(255, 0, 0);

    digitalWrite(PinDefinitions::PIN_UNLOCK, HIGH);

    delay(100);

    digitalWrite(PinDefinitions::PIN_UNLOCK, LOW);

    delay(500);

//    _espNeopixel->SetBuiltInLedRGB(0, 0, 0);
}

void CarHelper::UnlockAllDoors()
{
//    _espNeopixel->SetBuiltInLedRGB(255, 0, 0);

    digitalWrite(PinDefinitions::PIN_UNLOCK, HIGH);

    delay(100);

    digitalWrite(PinDefinitions::PIN_UNLOCK, LOW);

    delay(300);

    digitalWrite(PinDefinitions::PIN_UNLOCK, HIGH);

    delay(100);

    digitalWrite(PinDefinitions::PIN_UNLOCK, LOW);

    delay(500);

//    _espNeopixel->SetBuiltInLedRGB(0, 0, 0);
}

void CarHelper::Lock()
{
//   _espNeopixel->SetBuiltInLedRGB(0, 255, 0);

    digitalWrite(PinDefinitions::PIN_LOCK, HIGH);

    delay(100);

    digitalWrite(PinDefinitions::PIN_LOCK, LOW);

    delay(500);

//    _espNeopixel->SetBuiltInLedRGB(0, 0, 0);
}
