#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H


#include <Arduino.h>
#include "EspBoardHelpers/PinDefinitions.h"

class CarHelper
{
public:
    void UnlockDriverDoor();

    void UnlockAllDoors();

    void Lock();

};


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
