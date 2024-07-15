#include <Arduino.h>
#include <ESP32Time.h>
#include <Secrets/Secrets.h>
#include "EspBoardHelpers/EspNeopixel.h"
#include "EspBoardHelpers/SketchInitializers.h"
#include "Nfc/Pn532ShieldHandler.h"
#include "CarHelper/CarHelper.h"
#include "Logging/Logger.h"

//#define DEBUG_SERIAL_ON

// Uncomment one:
const String whichCar = "CAR_IS_2008_HONDA";
//const String whichCar = "CAR_IS_2003_COROLLA";
//const String whichCar = "CAR_IS_2021_COROLLA";

#define microsToMillis 10000                    /* CoSnversion factor for micro seconds to milliseconds */

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  3        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

RTC_DATA_ATTR unsigned long long epochAtLastRead = 0;
RTC_DATA_ATTR unsigned long long epochAtLastDoorOpened = 0;

RTC_DATA_ATTR bool carLockedOnceFlag = false;
RTC_DATA_ATTR bool carLockedTwiceFlag = false;

// Dependency setup
auto logger = *new Logger(LogLevel::Information); // Change this to fatal for what probably amounts to an imperceptibly faster time

ESP32Time espRtc(0);
auto sketchInitializers = *new SketchInitializers();
auto carHelper = *new CarHelper(whichCar);
auto pn532ShieldHandler = *new Pn532ShieldHandler(&logger, &carHelper, &epochAtLastRead, &espRtc);

void checkDomeLight();

void setup()
{
    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    pinMode(10, INPUT);
    pinMode(1, INPUT_PULLUP);

    // // For power savings, set all pins not being used as inputs to outputs. It works.
    // for (int i = 0; i < 17; i++)
    // {
    //   if (i == 2){ i = 3; } // Skip ss pin
    //   if (i == 10){ i = 12; } // Skip lock/unlock pins
    //
    //   pinMode(i, OUTPUT);
    // }

#ifdef DEBUG_SERIAL_ON
    Serial.begin(115200);
#endif

    sketchInitializers.InitializeSpiPins();

    sketchInitializers.Honda2008InitializeRemotePins();

    logger.Information("Checking for tag and sleeping");

    bootCount = bootCount + 1;

    checkDomeLight();

    pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::authorizedNfcTags, true);
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

void checkDomeLight(const String &whichCar)
{
    const unsigned long long localEpoch = espRtc.getLocalEpoch();

    const unsigned long long secondsSinceLastDoorOpen = localEpoch - epochAtLastDoorOpened;

    int domePinRead = 999;
    bool doorOpen = false;

    if (whichCar == "CAR_IS_2008_HONDA")
    {
        domePinRead = analogRead(10);
        doorOpen = domePinRead < 460;
    }

#ifdef DEBUG_SERIAL_ON
    Serial.println();
    Serial.println();

    Serial.print("domePinRead: ");
    Serial.println(domePinRead);

    Serial.print("doorOpen: ");
    Serial.println(doorOpen);

    Serial.print("Current RTC epoch: ");
    Serial.println(localEpoch);

    Serial.print("secondsSinceLastDoorOpen: ");
    Serial.println(secondsSinceLastDoorOpen);
#endif

    if (!carLockedOnceFlag &&
        secondsSinceLastDoorOpen > 30)
    {
        CarHelper::LockAllDoors();

        carLockedOnceFlag = true;
    }

    if (!carLockedTwiceFlag &&
        secondsSinceLastDoorOpen > 60)
    {
        CarHelper::LockAllDoors();

        carLockedTwiceFlag = true;
    }

    if (doorOpen)
    {
        epochAtLastDoorOpened = espRtc.getLocalEpoch();
        pn532ShieldHandler.EpochOfLastReset = espRtc.getLocalEpoch();

        carLockedOnceFlag = false;
        carLockedTwiceFlag = false;
    }

#ifdef DEBUG_SERIAL_ON
    Serial.print("Pin 1: ");
    Serial.println(digitalRead(1));

    Serial.println();
    Serial.println();
#endif
}
