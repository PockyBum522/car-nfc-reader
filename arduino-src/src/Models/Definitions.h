#ifndef COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H
#define COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H


class Definitions
{
public:
    static int PIN_UNLOCK;
    static int PIN_LOCK;
    static int PIN_TRUNK;

    static constexpr int PIN_PN532_BOARD_POWER = 15;

    static constexpr int PIN_DOME_LIGHT = 10;

	static constexpr int PIN_BTN_OPT_01 = 4;
	static constexpr int PIN_SW_OPT_02 = 1;
	static constexpr int PIN_SW_OPT_03 = 2;

	// 2008 Honda:
    static constexpr int CAR_REMOTE_ACTIVE = HIGH;
    static constexpr int CAR_REMOTE_INACTIVE = LOW;
};

// Give defaults
int Definitions::PIN_UNLOCK = 41;
int Definitions::PIN_LOCK = 42;
int Definitions::PIN_TRUNK = 40;


#endif //COROLLA_2003_NFC_CONTROLLER_PINDEFINITIONS_H
