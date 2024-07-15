#ifndef COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H
#define COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H

#include <Arduino.h>
#include <ESP32Time.h>
#include <vector>
#include "CarHelper/CarHelper.h"
#include "External/Adafruit PN532/Adafruit_PN532.h"
#include "Models/NfcTag.h"
#include "Logging/Logger.h"

#define PN532_MOSI 5
#define PN532_SS   6
#define PN532_MISO 7
#define PN532_SCK  8

class Pn532ShieldHandler
{
public:
    explicit Pn532ShieldHandler(Logger *logger, CarHelper *carHelper, const unsigned long long *epochAtLastRead, ESP32Time *espTime);

    void CheckForNfcTag(const std::vector<NfcTag>& vector);

    void checkAuthentication(uint8_t uid[7], uint8_t length, const std::vector<NfcTag>& vector);

    bool InitializeNfcShield(bool checkVersionData = false);

    void CheckForNfcTagAndPowerBackDown(const std::vector<NfcTag>& vector, bool checkVersionData = false);

    unsigned long long IncreasingDelayWithTime() const;

    unsigned long long EpochOfLastReset;

private:

    Adafruit_PN532 *_nfc;

    char msg[50]{};
    char converted[50]{};

    String decToHex(byte decValue, byte desiredStringLength);

    CarHelper *_carHelper;

    ESP32Time *_espRtc;

    Logger *_logger;
};


#endif //COROLLA_2003_NFC_CONTROLLER_PN532SHIELDHANDLER_H
