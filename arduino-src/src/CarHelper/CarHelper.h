#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H


#include <Arduino.h>
#include "EspBoardHelpers/Definitions.h"

class CarHelper
{
public:
    void UnlockDriverDoor();

    void Honda2008UnlockAllDoors();
    void Honda2008LockAllDoors();
    void Corolla2003UnlockAllDoors();

    void Lock();

};


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
