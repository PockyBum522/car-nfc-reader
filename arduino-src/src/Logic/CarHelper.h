#ifndef COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
#define COROLLA_2003_NFC_CONTROLLER_CARHELPER_H


#include <Arduino.h>

#include "EspEpochHelper.h"
#include "../Models/Definitions.h"
#include "Models/WhichCar.h"

RTC_DATA_ATTR bool m_carLockedOnceFlag = false;
RTC_DATA_ATTR bool m_carLockedTwiceFlag = false;


class CarHelper
{
public:
    static int TrunkOpenCounter;

    explicit CarHelper(const WhichCar whichCarIn, EspEpochHelper *espEpochHelper, const bool debugSerialOn):
        m_debugSerialOn(debugSerialOn),
        m_epochHelper(espEpochHelper),
        m_whichCar(whichCarIn) { }

    void Loop() const
    {
        checkDomeLight();

        checkTrunkOpenCounter();

        checkQueuedActions();
    }

    void UnlockAllDoors() const
    {
        if (m_unlockAllDoorsStartedAt != 0)
            return;

        m_unlockAllDoorsStartedAt = m_epochHelper->GetMillisecondsSinceBoardPowerOn();
    }

    void LockAllDoors() const
    {
        if (m_lockAllDoorsStartedAt != 0)
            return;

        m_lockAllDoorsStartedAt = m_epochHelper->GetMillisecondsSinceBoardPowerOn();
    }

    void OpenTrunk() const
    {
        if (m_openTrunkStartedAt != 0)
            return;

        m_openTrunkStartedAt = m_epochHelper->GetMillisecondsSinceBoardPowerOn();
    }

    static std::string WhichCarToString(const WhichCar whichCar)
    {
        if (whichCar == WhichCar::HONDA_2008)
            return "Honda Civic Si";

        if (whichCar == WhichCar::COROLLA_2003)
            return "2003 Corolla";

        if (whichCar == WhichCar::COROLLA_2021)
            return "2021 Corolla";

        return "ERROR";
    }

private:
    EspEpochHelper *m_epochHelper;
    const WhichCar m_whichCar;

    bool m_debugSerialOn;

    static unsigned long long m_unlockAllDoorsStartedAt;
    static unsigned long long m_lockAllDoorsStartedAt;
    static unsigned long long m_openTrunkStartedAt;

    void checkQueuedActions() const
    {
        if (m_unlockAllDoorsStartedAt != 0)
            workUnlockAllDoors();

        if (m_lockAllDoorsStartedAt != 0)
            workLockAllDoors();

        if (m_openTrunkStartedAt != 0)
            workOpenTrunk();
    }

    void checkDomeLight() const
    {
        int domePinRead = 999;
        bool doorOpen = false;

        if (m_whichCar == WhichCar::HONDA_2008)
        {
            domePinRead = analogRead(Definitions::PIN_DOME_LIGHT);
            doorOpen = domePinRead < 460;
        }

        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            domePinRead = analogRead(Definitions::PIN_DOME_LIGHT);
            doorOpen = domePinRead < 460;
        }

        if (!m_carLockedOnceFlag &&
            m_epochHelper->GetSecondsSinceDoorLastOpen() > 30)
        {
            LockAllDoors();

            m_carLockedOnceFlag = true;
        }

        if (!m_carLockedTwiceFlag &&
            m_epochHelper->GetSecondsSinceDoorLastOpen() > 65)
        {
            LockAllDoors();

            m_carLockedTwiceFlag = true;
        }

        if (doorOpen)
        {
            if (m_epochHelper->GetSecondsSinceDoorLastOpen() > 300)
            {
                // Someone either drove somewhere or used the keyfob to unlock the car
                //      reset this so that the Wi-Fi comes back on for a bit
                m_epochHelper->ResetSecondsSinceBoardPowerOn();
            }

            m_epochHelper->SetDoorLastOpenAtToNow();

            m_carLockedOnceFlag = false;
            m_carLockedTwiceFlag = false;
        }

        if (!m_debugSerialOn) return;

        // Otherwise:
        Serial.println();
        Serial.println();

        Serial.print("domePinRead: ");
        Serial.println(domePinRead);

        Serial.print("doorOpen: ");
        Serial.println(doorOpen);
    }

    void checkTrunkOpenCounter() const
    {
        if (TrunkOpenCounter > 0)
            TrunkOpenCounter--;

        if (TrunkOpenCounter > 1000)
        {
            TrunkOpenCounter = 0;

            OpenTrunk();
        }

        if (!m_debugSerialOn)
            return;

        // Otherwise
        Serial.print("trunkOpenCounter: ");
        Serial.println(TrunkOpenCounter);
    }

    void workUnlockAllDoors() const
    {
        unsigned long long elapsed = m_epochHelper->GetMillisecondsSinceBoardPowerOn() - m_unlockAllDoorsStartedAt;

        if (m_whichCar == WhichCar::HONDA_2008)
        {
            if (elapsed > 0 && elapsed < 250)
            {
                pinMode(Definitions::PIN_UNLOCK, OUTPUT);
                digitalWrite(Definitions::PIN_UNLOCK, LOW);

                return;
            }

            if (elapsed > 250 && elapsed < 500)
            {
                pinMode(Definitions::PIN_UNLOCK, INPUT);

                return;
            }

            if (elapsed > 500 && elapsed < 750)
            {
                pinMode(Definitions::PIN_UNLOCK, OUTPUT);
                digitalWrite(Definitions::PIN_UNLOCK, LOW);

                return;
            }

            if (elapsed > 750 && elapsed < 1000)
            {
                pinMode(Definitions::PIN_UNLOCK, INPUT);

                return;
            }
        }

        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            if (elapsed > 0 && elapsed < 400)
            {
                neopixelWrite(RGB_BUILTIN, 0, 20, 0);

                pinMode(Definitions::PIN_UNLOCK, OUTPUT);
                digitalWrite(Definitions::PIN_UNLOCK, LOW);

                return;
            }

            if (elapsed > 400 && elapsed < 800)
            {
                neopixelWrite(RGB_BUILTIN, 0, 0, 0);

                pinMode(Definitions::PIN_UNLOCK, INPUT);

                return;
            }

            if (elapsed > 800 && elapsed < 1200)
            {
                neopixelWrite(RGB_BUILTIN, 0, 20, 0);

                pinMode(Definitions::PIN_LOCK, OUTPUT);
                digitalWrite(Definitions::PIN_LOCK, LOW);

                return;
            }

            if (elapsed > 1200 && elapsed < 1600)
            {

                pinMode(Definitions::PIN_LOCK, INPUT);

                return;
            }
        }

        if (elapsed > 3000)
        {
            neopixelWrite(RGB_BUILTIN, 0, 0, 0);

            m_unlockAllDoorsStartedAt = 0;
        }
    }

    void workLockAllDoors() const
    {
        unsigned long long elapsed = m_epochHelper->GetMillisecondsSinceBoardPowerOn() - m_lockAllDoorsStartedAt;

        if (m_whichCar == WhichCar::HONDA_2008)
        {
            if (elapsed > 0 && elapsed < 250)
            {
                pinMode(Definitions::PIN_LOCK, OUTPUT);
                digitalWrite(Definitions::PIN_LOCK, LOW);

                return;
            }

            if (elapsed > 250 && elapsed < 500)
            {
                pinMode(Definitions::PIN_LOCK, INPUT);

                return;
            }
        }

        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            if (elapsed > 0 && elapsed < 400)
            {
                pinMode(Definitions::PIN_LOCK, OUTPUT);
                digitalWrite(Definitions::PIN_LOCK, LOW);

                return;
            }

            if (elapsed > 400 && elapsed < 800)
            {
                pinMode(Definitions::PIN_LOCK, INPUT);

                return;
            }
        }

        if (elapsed > 3000)
        {
            m_lockAllDoorsStartedAt = 0;
        }
    }

    void workOpenTrunk() const
    {
        unsigned long long elapsed = m_epochHelper->GetMillisecondsSinceBoardPowerOn() - m_openTrunkStartedAt;

        if (m_whichCar == WhichCar::HONDA_2008)
        {
            if (elapsed > 0 && elapsed < 600)
            {
                pinMode(Definitions::PIN_TRUNK, OUTPUT);
                digitalWrite(Definitions::PIN_TRUNK, LOW);

                return;
            }

            if (elapsed > 600 && elapsed < 1200)
            {
                pinMode(Definitions::PIN_TRUNK, INPUT);

                return;
            }
        }

        if (m_whichCar == WhichCar::COROLLA_2021)
        {
            if (elapsed > 0 && elapsed < 600)
            {
                pinMode(Definitions::PIN_TRUNK, OUTPUT);
                digitalWrite(Definitions::PIN_TRUNK, LOW);

                return;
            }

            if (elapsed > 600 && elapsed < 1200)
            {
                pinMode(Definitions::PIN_TRUNK, INPUT);

                return;
            }
        }

        if (elapsed > 3000)
        {
            m_openTrunkStartedAt = 0;
        }
    }
};

int CarHelper::TrunkOpenCounter = 0;

unsigned long long CarHelper::m_unlockAllDoorsStartedAt = 0;
unsigned long long CarHelper::m_lockAllDoorsStartedAt = 0;
unsigned long long CarHelper::m_openTrunkStartedAt = 0;


#endif //COROLLA_2003_NFC_CONTROLLER_CARHELPER_H
