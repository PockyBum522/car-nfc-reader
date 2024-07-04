#include <Arduino.h>
#include <vector>
#include <ESP32Time.h>
#include <Secrets/Secrets.h>

#include "EspBoardHelpers/EspNeopixel.h"
#include "EspBoardHelpers/SketchInitializers.h"
#include "Nfc/Pn532ShieldHandler.h"
#include "CarHelper/CarHelper.h"
#include "Models/NfcTag.h"
#include "Logging/Logger.h"


// Constants
#define microsToMillis 10000                    /* Conversion factor for micro seconds to milliseconds */


// RTC memory persistent variables
RTC_DATA_ATTR unsigned long long epochAtLastRead = 0;
RTC_DATA_ATTR unsigned long long epochAtDomeLightLastOn = 0;

RTC_DATA_ATTR bool carLockedOnceFlag = false;
RTC_DATA_ATTR bool carLockedTwiceFlag = false;

// Dependency setup
auto logger = *new Logger(LogLevel::Debug);

ESP32Time espRtc(0);
auto sketchInitializers = *new SketchInitializers();
auto carHelper = *new CarHelper();
auto pn532ShieldHandler = *new Pn532ShieldHandler(&logger, &carHelper, &epochAtLastRead);


void LockBasedOnDomeLightVoltageLoop();
unsigned long long IncreasingDelayWithTimeMilliseconds();

void setup()
{
    Serial.begin(115200);

    sketchInitializers.InitializeSpiPins();

    pn532ShieldHandler.InitializeNfcShield(true);
}

void loop()
{
    delay(100); // Necessary to prevent hang

    pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::authorizedNfcTags, true);
}


unsigned long long IncreasingDelayWithTimeMilliseconds()
{
    static constexpr unsigned long HOURS = 60 * 60;
    static constexpr unsigned long DAYS = 24 * HOURS;

    constexpr unsigned long long delayMillisConstant = 0.006;

    const unsigned long long secondsSinceLastRead = espRtc.getLocalEpoch() - epochAtLastRead;

    const unsigned long long delayMillis = secondsSinceLastRead * delayMillisConstant;

    logger.Information("Seconds since last read: " + String(secondsSinceLastRead));
    logger.Information("delayMillis: " + String(delayMillis));

    //if (delayMillis < 100)  return 100;

    if (delayMillis < 3000) return delayMillis;

    else                    return 4000;

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

void LockBasedOnDomeLightVoltageLoop()
{
    const int MINUTES = 60;

    unsigned long long rtcLocalEpoch = espRtc.getLocalEpoch();
    unsigned long long secondsSinceDomeLightLastOn = espRtc.getLocalEpoch() - epochAtDomeLightLastOn;

    bool domeLightState = !digitalRead(PinDefinitions::PIN_DOME_LIGHT);

    if (domeLightState == HIGH)
    {
        epochAtLastRead = rtcLocalEpoch;
        epochAtDomeLightLastOn = rtcLocalEpoch;

        carLockedOnceFlag = false;
        carLockedTwiceFlag = false;

        logger.Debug("Dome light state: ON");
    }

    if (carLockedTwiceFlag) return;

    if (secondsSinceDomeLightLastOn > 4 * MINUTES)
    {
        carHelper.Lock();
        carLockedTwiceFlag = true;

        logger.Debug("Dome light timeout: Locking car for the second time");
    }

    if (carLockedOnceFlag) return;

    if (secondsSinceDomeLightLastOn > 2)
    {
        carHelper.Lock();
        carLockedOnceFlag = true;

        logger.Debug("Dome light timeout: Locking car for the first time");
    }
}
