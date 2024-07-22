#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H

#include <Arduino.h>
#include "EspBoardHelpers/Definitions.h"

class CarHelper
{
public:
    explicit CarHelper(String *whichCarIn);

    void UnlockAllDoors() const;
    void LockAllDoors() const;
    void OpenTrunk() const;

private:
    String *whichCar;
};


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
