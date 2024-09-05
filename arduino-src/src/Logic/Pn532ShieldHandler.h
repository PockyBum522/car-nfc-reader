#ifndef COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H
#define COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H

#include <Arduino.h>
#include <ESP32Time.h>
#include <vector>
#include "CarHelper.h"
#include "Models/NfcTag.h"

#define PN532_MOSI 5
#define PN532_SS   6
#define PN532_MISO 7
#define PN532_SCK  8

class Pn532ShieldHandler
{
public:
    explicit Pn532ShieldHandler(CarHelper *carHelper, Adafruit_PN532 *nfcReader, EspEpochHelper *epochHelper, bool debugSerialOn)
    {
        pinMode(48, OUTPUT);

        m_debugSerialOn = debugSerialOn;
        m_nfc = nfcReader;
        m_carHelper = carHelper;
        m_epochHelper = epochHelper;
    }

    void CheckForNfcTagAndPowerBackDown(const std::vector<NfcTag> &nfcTags, bool checkVersionData, bool deepSleep)
    {
        digitalWrite(Definitions::PIN_PN532_BOARD_POWER, HIGH);

        unsigned long long millisBeforeNfcInit = millis();

        delay(9); // Makes init almost twice as fast.

        m_nfc->wakeup();

        bool unused = InitializeNfcShield(true);

        CheckForNfcTag(nfcTags);

        Serial.println("NFC Init took: " + String(millis() - millisBeforeNfcInit) + " millis");

        unsigned long long microsToSleep = IncreasingDelayWithTime();

        if (m_debugSerialOn)
        {
            Serial.println(String("about to sleep for x microseconds: " + String(microsToSleep)));
        }

        digitalWrite(Definitions::PIN_PN532_BOARD_POWER, LOW);

        if (deepSleep)
        {
            esp_sleep_enable_timer_wakeup(microsToSleep);

            esp_deep_sleep_start();
        }
    }

    void CheckForNfcTag(const std::vector<NfcTag>& nfcTags)
    {
        boolean success;
        uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
        uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

        // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
        // 'uid' will be populated with the UID, and uidLength will indicate
        // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
        success = m_nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

        if (success)
        {
            m_epochHelper->SetDoorLastOpenAtToNow();        // Reset this so we can use it if there's no read later and the light comes on, we know someone drove somewhere
            m_epochHelper->SetLastReadToNow();

            checkAuthentication(uid, uidLength, nfcTags);
        }
    }

    void checkAuthentication(uint8_t uid[7], uint8_t uidLength, const std::vector<NfcTag>& nfcTags)
    {
        String UIDString = "";

        for (uint8_t i = 0; i < uidLength; i++)
        {
            UIDString += decToHex(uid[i], 2);

            if (i < uidLength - 1)
            {
                UIDString += ':';
            }
        }

        UIDString.toUpperCase();

        // This is to convert it to a char array for the snprintf
        for (int i = 0; i < UIDString.length(); ++i)
        {
            if (m_converted[i] != ':')
            {
                m_converted[i] = UIDString[i];
            }
        }

        snprintf (m_msg, 50, "%s", m_converted);

        Serial.println("New UID read: " + String(m_msg));

        for (const auto& tag : nfcTags)
        {
            if (!tag.Uid.equals(m_msg)) continue;

            // Otherwise, we matched and authenticated!

            if (m_debugSerialOn)
                Serial.println(tag.Username + " seen! Unlocking car now.");

            m_carHelper->UnlockAllDoors();

            CarHelper::TrunkOpenCounter += 100;
        }
    }

    bool InitializeNfcShield(bool checkVersionData = false) const
    {
        m_nfc->begin();

        if (checkVersionData)
        {
            uint32_t versiondata = m_nfc->getFirmwareVersion();

            if (!versiondata)
            {
                if (m_debugSerialOn)
                    Serial.println("Didn't find PN53x board");

                return false;
            }

            if (m_debugSerialOn)
            {
                Serial.println("Found chip PN5" + String((versiondata>>24) & 0xFF, HEX));
                Serial.println("Firmware ver: " + String((versiondata>>16) & 0xFF, DEC) + "." + String((versiondata>>8) & 0xFF, DEC));
            }
        }

        // Set the max number of retry attempts to read from a card, preventing us from waiting forever for a card, which is the default behaviour of the PN532
        m_nfc->setPassiveActivationRetries(0x01);

        return true;
    }

    unsigned long long IncreasingDelayWithTime() const
    {
        const unsigned long long localEpoch = m_epochHelper->GetSecondsSinceBoardPowerOn();

        constexpr int multipleForMicros = 1000;

        if (m_debugSerialOn)
        {
            Serial.println();
            Serial.println(String("localEpoch: ") + String(localEpoch));
            Serial.println(String("secondsSinceLastReset: ") + String(localEpoch));
            Serial.println(String("secondsSinceLastReset * 0.006: ") + String(localEpoch * 0.006));
            Serial.println(String("secondsSinceLastReset * 0.006 * multipleForMicros: ") + String(localEpoch * 0.006 * multipleForMicros));
        }

        // If less than 36 hours, return value multiplied by a smaller constant
        if (localEpoch < 129600)
            return localEpoch * 0.004 * multipleForMicros;

        // If more than 36, but less than 72 hours, delay will suddenly be slightly longer and increase slightly faster. At 72 hours, this will be 2.59 seconds
        if (localEpoch < 259200)
            return localEpoch * 0.01 * multipleForMicros;

        // If more than 72 hours, just eturn 3 seconds
        return 3000000;

        //    Hours	    Minutes	    Seconds     Delay millis with constant = 0.006
        //
        //    10	    600	        36000		216
        //    24	    1440        86400		518.4
        //    48	    2880        172800		1036.8
        //    72	    4320        259200		1555.2
        //    96	    5760        345600		2073.6
        //    120	    7200        432000		2592
        //    144	    8640        518400		3110.4
        //    168	    1008    	604800		3628.8
    }

private:
    Adafruit_PN532 *m_nfc;
    CarHelper *m_carHelper;
    EspEpochHelper *m_epochHelper;

    char m_msg[50]{};
    char m_converted[50]{};

    bool m_debugSerialOn;

    static String decToHex(byte decValue, byte desiredStringLength)
    {
        String hexString = String(decValue, HEX);
        while (hexString.length() < desiredStringLength) hexString = "0" + hexString;

        return hexString;
    }
};


#endif //COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H
