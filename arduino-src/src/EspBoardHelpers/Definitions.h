//
// Created by David on 5/24/2023.
//

#ifndef COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H
#define COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H

class Definitions
{
public:
    static constexpr int PIN_UNLOCK = 41;
    static constexpr int PIN_LOCK = 42;
    static constexpr int PIN_TRUNK = 40;

    static constexpr int PIN_PN532_BOARD_POWER = 15;

    static constexpr int PIN_IMU_BOARD_POWER = 47;

    static constexpr int PIN_DOME_LIGHT = 10;

	// 2003 Corolla:

	// 2008 Honda:
    static constexpr int CAR_REMOTE_ACTIVE = HIGH;
    static constexpr int CAR_REMOTE_INACTIVE = LOW;

};

#endif //COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H
