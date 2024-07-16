#include "Pn532ShieldHandler.h"

#include <ESP32Time.h>

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

Pn532ShieldHandler::Pn532ShieldHandler(Logger *logger, CarHelper *carHelper, const unsigned long long *epochAtLastRead, ESP32Time *espTime)
{
    pinMode(48, OUTPUT);

    _logger = logger;
    _carHelper = carHelper;
    EpochOfLastReset = *epochAtLastRead;
    _espRtc = espTime;

    _nfc = new Adafruit_PN532(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
}

void Pn532ShieldHandler::CheckForNfcTagAndPowerBackDown(const std::vector<NfcTag>& nfcTags, bool checkVersionData)
{
    digitalWrite(Definitions::PIN_PN532_BOARD_POWER, HIGH);

    unsigned long long millisBeforeNfcInit = millis();

    delay(9); // Makes init almost twice as fast.

    _nfc->wakeup();

    InitializeNfcShield(true);

    CheckForNfcTag(nfcTags);

    _nfc->shutDown();

    _logger->Debug("NFC Init took: " + String(millis() - millisBeforeNfcInit) + " millis");

    digitalWrite(Definitions::PIN_PN532_BOARD_POWER, LOW);

    esp_sleep_enable_timer_wakeup(IncreasingDelayWithTime() * uS_TO_S_FACTOR);

    esp_deep_sleep_start();
}


unsigned long long Pn532ShieldHandler::IncreasingDelayWithTime() const
{
    const unsigned long long localEpoch = _espRtc->getLocalEpoch();

    const unsigned long long secondsSinceLastReset = localEpoch - EpochOfLastReset;


    // If less than 24 hours, return value multiplied by a smaller constant
    if (secondsSinceLastReset < 86400)
        return secondsSinceLastReset * 0.006;

    // If less than 72 hours, return value multiplied by the delay constant
    if (secondsSinceLastReset < 259200)
        return secondsSinceLastReset * 0.01;

    // If more than 72 hours, return 4 seconds
    return 4;


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

void Pn532ShieldHandler::CheckForNfcTag(const std::vector<NfcTag>& nfcTags)
{
    boolean success;
    uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };	// Buffer to store the returned UID
    uint8_t uidLength;				// Length of the UID (4 or 7 bytes depending on ISO14443A card type)

    // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
    // 'uid' will be populated with the UID, and uidLength will indicate
    // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
    success = _nfc->readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

    if (success)
    {
        EpochOfLastReset = _espRtc->getLocalEpoch();

        checkAuthentication(uid, uidLength, nfcTags);
    }
}

void Pn532ShieldHandler::checkAuthentication(uint8_t uid[7], uint8_t uidLength, const std::vector<NfcTag>& nfcTags)
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
        if (converted[i] != ':')
        {
            converted[i] = UIDString[i];
        }
    }

    snprintf (msg, 50, "%s", converted);

#ifdef DEBUG_SERIAL_ON
    Serial.println("New UID read: " + String(msg));
#endif

    for (const auto& tag : nfcTags)
    {
        if (!tag.Uid.equals(msg)) continue;

        // Otherwise, we matched and authenticated!

#ifdef DEBUG_SERIAL_ON
        Serial.println(tag.Username + " seen! Unlocking car now.");
#endif

        _carHelper->UnlockAllDoors();

        delay(1000);
    }
}

String Pn532ShieldHandler::decToHex(byte decValue, byte desiredStringLength)
{
    String hexString = String(decValue, HEX);
    while (hexString.length() < desiredStringLength) hexString = "0" + hexString;

    return hexString;
}

bool Pn532ShieldHandler::InitializeNfcShield(bool checkVersionData)
{
    _nfc->begin();

    if (checkVersionData)
    {
        uint32_t versiondata = _nfc->getFirmwareVersion();

        if (!versiondata)
        {
            _logger->Warning("Didn't find PN53x board");

            return false;
        }

        _logger->Information("Found chip PN5" + String((versiondata>>24) & 0xFF, HEX));
        _logger->Information("Firmware ver: " + String((versiondata>>16) & 0xFF, DEC) + "." + String((versiondata>>8) & 0xFF, DEC));
    }

    // Set the max number of retry attempts to read from a card, preventing us from waiting forever for a card, which is the default behaviour of the PN532
    _nfc->setPassiveActivationRetries(0x01);

    return true;
}

