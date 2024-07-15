#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H

#include <Arduino.h>
#include "EspBoardHelpers/Definitions.h"

class CarHelper
{
public:
    static void UnlockAllDoors();
    static void LockAllDoors();

    explicit CarHelper(const String &whichCar);

private:
    static String _whichCar;
};


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
