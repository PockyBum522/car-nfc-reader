#include <Arduino.h>
#include <ESP32Time.h>
#include <Secrets/Secrets.h>
#include "EspBoardHelpers/EspNeopixel.h"
#include "EspBoardHelpers/SketchInitializers.h"
#include "Nfc/Pn532ShieldHandler.h"
#include "CarHelper/CarHelper.h"
#include "Logging/Logger.h"


// Constants
#define microsToMillis 10000                    /* Conversion factor for micro seconds to milliseconds */


#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

// RTC memory persistent variables
RTC_DATA_ATTR unsigned long long epochAtLastRead = 0;
RTC_DATA_ATTR unsigned long long epochAtDomeLightLastOn = 0;

RTC_DATA_ATTR bool carLockedOnceFlag = false;
RTC_DATA_ATTR bool carLockedTwiceFlag = false;

bool runOnce = false;

// Dependency setup
auto logger = *new Logger(LogLevel::Information); // Change this to fatal for what probably amounts to an imperceptibly faster time

ESP32Time espRtc(0);
auto sketchInitializers = *new SketchInitializers();
auto carHelper = *new CarHelper();
auto pn532ShieldHandler = *new Pn532ShieldHandler(&logger, &carHelper, &epochAtLastRead, &espRtc);

void LockBasedOnDomeLightVoltageLoop();
unsigned long long IncreasingDelayWithTimeMilliseconds();

void setup()
{
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    // // For power savings, set all pins not being used as inputs to outputs. It works.
    // for (int i = 0; i < 17; i++)
    // {
    //   if (i == 2){ i = 3; } // Skip ss pin
    //   if (i == 10){ i = 12; } // Skip lock/unlock pins
    //
    //   pinMode(i, OUTPUT);
    // }

    Serial.begin(115200);

    sketchInitializers.InitializeSpiPins();

    sketchInitializers.Honda2008InitializeRemotePins();

    logger.Information("Checking for tag and sleeping");

    bootCount = bootCount + 1;

    //pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::authorizedNfcTags, true);
}

void loop()
{
    // // If it hasn't been 12 hours yet, just run a constant loop
    // while (IncreasingDelayWithTimeMilliseconds() < 250)

    // Testing @ 30min
    // while (IncreasingDelayWithTimeMilliseconds() < 10)
    // {
    //     if (!runOnce)
    //     {
    //         delay(100);
    //
    //         pn532ShieldHandler.InitializeNfcShield(true);
    //
    //         runOnce = true;
    //     }
    //
    //     logger.Information("Checking in while loop");
    //     pn532ShieldHandler.CheckForNfcTag(Secrets::authorizedNfcTags);
    // }

}

void LockBasedOnDomeLightVoltageLoop()
{
    // const int MINUTES = 60;
    //
    // unsigned long long rtcLocalEpoch = espRtc.getLocalEpoch();
    // unsigned long long secondsSinceDomeLightLastOn = espRtc.getLocalEpoch() - epochAtDomeLightLastOn;
    //
    // bool domeLightState = !digitalRead(Definitions::PIN_DOME_LIGHT);
    //
    // if (domeLightState == HIGH)
    // {
    //     epochAtLastRead = rtcLocalEpoch;
    //     epochAtDomeLightLastOn = rtcLocalEpoch;
    //
    //     carLockedOnceFlag = false;
    //     carLockedTwiceFlag = false;
    //
    //     logger.Debug("Dome light state: ON");
    // }
    //
    // if (carLockedTwiceFlag) return;
    //
    // if (secondsSinceDomeLightLastOn > 4 * MINUTES)
    // {
    //     carHelper.Lock();
    //     carLockedTwiceFlag = true;
    //
    //     logger.Debug("Dome light timeout: Locking car for the second time");
    // }
    //
    // if (carLockedOnceFlag) return;
    //
    // if (secondsSinceDomeLightLastOn > 2)
    // {
    //     carHelper.Lock();
    //     carLockedOnceFlag = true;
    //
    //     logger.Debug("Dome light timeout: Locking car for the first time");
    // }
}
