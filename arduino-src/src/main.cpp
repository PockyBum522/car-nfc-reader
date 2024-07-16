#include <Arduino.h>
#include <ESP32Time.h>
#include <Secrets/Secrets.h>
#include "EspBoardHelpers/EspNeopixel.h"
#include "EspBoardHelpers/SketchInitializers.h"
#include "Nfc/Pn532ShieldHandler.h"
#include "CarHelper/CarHelper.h"
#include "Logging/Logger.h"
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ElegantOTA.h>

bool debugSerialOn = false;

// Uncomment one:
String whichCar = "2008_HONDA";
//String whichCar = "2003_COROLLA";
//String whichCar = "2021_COROLLA";

// How long until it stops being able to get OTA updates after a reset
constexpr int wifiPortalTimeout = 1200;    // 1200s = 20 minutes

#define uS_TO_S_FACTOR 1000000  /* Conversion factor for micro seconds to seconds */

RTC_DATA_ATTR unsigned long long epochAtLastRead = 0;
RTC_DATA_ATTR unsigned long long epochAtLastDoorOpened = 0;

RTC_DATA_ATTR bool carLockedOnceFlag = false;
RTC_DATA_ATTR bool carLockedTwiceFlag = false;

// Dependency setup
auto logger = *new Logger(Information, &debugSerialOn);

ESP32Time espRtc(0);
auto sketchInitializers = *new SketchInitializers();
auto carHelper = *new CarHelper(&whichCar);
auto pn532ShieldHandler = *new Pn532ShieldHandler(&logger, &carHelper, &epochAtLastRead, &espRtc, debugSerialOn);

WiFiManager wifiManager;
WebServer server(80);

void checkDomeLight(const String &whichCar);

void setup()
{
    if (debugSerialOn)
        Serial.begin(115200);

    pinMode(15, OUTPUT);
    digitalWrite(15, HIGH);

    pinMode(10, INPUT);
    pinMode(1, INPUT_PULLUP);

    if (espRtc.getLocalEpoch() < wifiPortalTimeout)
    {
        // If less than 10 minutes since last board reset, let wifiManager do its thing
        wifiManager.setConfigPortalBlocking(false);
        wifiManager.setMinimumSignalQuality(20);
        wifiManager.autoConnect(whichCar.c_str(), Secrets::WifiPsk.c_str());

        server.on("/", []() {
            server.send(200, "text/plain", "Hi! This is " + whichCar);
        });

        ElegantOTA.begin(&server);    // Start ElegantOTA
        server.begin();

        server.handleClient();
        ElegantOTA.loop();
    }

    // // For power savings, set all pins not being used as inputs to outputs. It works.
    // for (int i = 0; i < 17; i++)
    // {
    //   if (i == 2){ i = 3; } // Skip ss pin
    //   if (i == 10){ i = 12; } // Skip lock/unlock pins
    //
    //   pinMode(i, OUTPUT);
    // }

    sketchInitializers.InitializeSpiPins();

    sketchInitializers.Honda2008InitializeRemotePins();
}

void loop()
{
    if (espRtc.getLocalEpoch() < wifiPortalTimeout)
    {
        // If less than 10 minutes since last board reset, let wifiManager do its thing then check for NFC tags and DON'T deep sleep the board
        wifiManager.process();

        server.handleClient();
        ElegantOTA.loop();

        checkDomeLight(whichCar);

        if (debugSerialOn)
        {
            Serial.print("UNDER 10m threshold time: ");
            Serial.println(espRtc.getLocalEpoch());
        }

        pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, false);
    }
    else
    {
        // If more than 10 minutes since last board reset, then just check for tag then deep sleep
        checkDomeLight(whichCar);

        if (debugSerialOn)
        {
            Serial.print("Over 10m threshold time, should be no more AP: ");
            Serial.println(espRtc.getLocalEpoch());
        }

        pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, true);
    }
}

void checkDomeLight(const String &whichCar)
{
    const unsigned long long localEpoch = espRtc.getLocalEpoch();

    const unsigned long long secondsSinceLastDoorOpen = localEpoch - epochAtLastDoorOpened;

    int domePinRead = 999;
    bool doorOpen = false;

    if (whichCar == "2008_HONDA")
    {
        domePinRead = analogRead(10);
        doorOpen = domePinRead < 460;
    }

    if (debugSerialOn)
    {
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
    }

    if (!carLockedOnceFlag &&
        secondsSinceLastDoorOpen > 30)
    {
        carHelper.LockAllDoors();

        carLockedOnceFlag = true;
    }

    if (!carLockedTwiceFlag &&
        secondsSinceLastDoorOpen > 60)
    {
        carHelper.LockAllDoors();

        carLockedTwiceFlag = true;
    }

    if (doorOpen)
    {
        epochAtLastDoorOpened = espRtc.getLocalEpoch();
        pn532ShieldHandler.EpochOfLastReset = espRtc.getLocalEpoch();

        carLockedOnceFlag = false;
        carLockedTwiceFlag = false;
    }

    if (debugSerialOn)
    {
        Serial.print("Pin 1: ");
        Serial.println(digitalRead(1));

        Serial.println();
        Serial.println();
    }
}
