#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Adafruit_SPIDevice.h>
#include <ESP32Time.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ElegantOTA.h>
#include "Secrets/Secrets.h"
#include "Models/Definitions.h"
#include "Logic/SketchInitializers.h"
#include "Logic/CarHelper.h"
#include "Logic/Pn532ShieldHandler.h"

bool debugSerialOn = true;

// Uncomment one:
//auto m_whichCar = WhichCar::HONDA_2008;
//auto m_whichCar = WhichCar::COROLLA_2003;
auto m_whichCar = WhichCar::COROLLA_2021;

// How long until it stops being able to get OTA updates after a reset
constexpr int m_wifiPortalTimeout = 1200;    // 1200s = 20 minutes

RTC_DATA_ATTR unsigned long long m_epochAtLastDoorOpened = 0;

RTC_DATA_ATTR bool m_carLockedOnceFlag = false;
RTC_DATA_ATTR bool m_carLockedTwiceFlag = false;

RTC_DATA_ATTR unsigned long long m_trunkOpenCounter = 0;

int Definitions::PIN_UNLOCK = 41;
int Definitions::PIN_LOCK = 42;
int Definitions::PIN_TRUNK = 40;

auto m_spiDevice = new Adafruit_SPIDevice(PN532_SS, PN532_SCK, PN532_MISO, PN532_MOSI,
    300000, SPI_BITORDER_LSBFIRST, SPI_MODE0);

Adafruit_PN532 m_nfcReader(PN532_SS, m_spiDevice);

ESP32Time m_espRtc(0);
auto m_sketchInitializers = *new SketchInitializers();
auto m_carHelper = *new CarHelper(m_whichCar);
auto m_pn532ShieldHandler = *new Pn532ShieldHandler(&m_carHelper, &m_nfcReader, &m_espRtc, &m_trunkOpenCounter, debugSerialOn);

WiFiManager m_wifiManager;
WebServer m_server(80);

void checkDomeLight(WhichCar whichCar);

void setup()
{
    Serial.begin(115200);

    pinMode(Definitions::PIN_PN532_BOARD_POWER, OUTPUT);
    digitalWrite(Definitions::PIN_PN532_BOARD_POWER, HIGH);

    pinMode(Definitions::PIN_DOME_LIGHT, INPUT);

    pinMode(Definitions::PIN_BTN_OPT_01, INPUT_PULLUP);
    pinMode(Definitions::PIN_SW_OPT_02, INPUT_PULLUP);
    pinMode(Definitions::PIN_SW_OPT_03, INPUT_PULLUP);

    SketchInitializers::InitializeSpiPins();

    SketchInitializers::InitializeRemotePins(m_whichCar);

    const std::string whichCarString = CarHelper::WhichCarToString(m_whichCar);

    if (m_espRtc.getLocalEpoch() < m_wifiPortalTimeout)
    {
        // If less than 10 minutes since last board reset, let wifiManager do its thing
        m_wifiManager.setConfigPortalBlocking(false);
        m_wifiManager.setMinimumSignalQuality(20);
        m_wifiManager.autoConnect(whichCarString.c_str(), Secrets::WifiPsk.c_str());

        m_server.on("/", []()
        {
            const std::string carString = CarHelper::WhichCarToString(m_whichCar);

            m_server.send(200, "text/plain", "Hi! This is " + String(carString.c_str()));
        });

        ElegantOTA.begin(&m_server);    // Start ElegantOTA
        m_server.begin();

        m_server.handleClient();
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
}

void checkTrunkOpenCounter()
{
//    Serial.print("trunkOpenCounter from main: ");
//    Serial.println(trunkOpenCounter);

     if (m_trunkOpenCounter > 0)
         m_trunkOpenCounter--;

     if (m_trunkOpenCounter > 250)
     {
         m_trunkOpenCounter = 0;

         m_carHelper.OpenTrunk();
     }
}

void loop()
{
    const bool button01State = !digitalRead(Definitions::PIN_BTN_OPT_01);
    const bool switch02State = !digitalRead(Definitions::PIN_SW_OPT_02);
    const bool switch03State = !digitalRead(Definitions::PIN_SW_OPT_03);

    // This all works great, also tested reboot with switches set to on in case accidentally strapping pins, board still boots fine:

    // Serial.println();
    // Serial.println();
    // Serial.print("button01State: ");
    // Serial.println(button01State);
    //
    // Serial.print("switch02State: ");
    // Serial.println(switch02State);
    //
    // Serial.print("switch03State: ");
    // Serial.println(switch03State);

    if (m_espRtc.getLocalEpoch() < m_wifiPortalTimeout)
    {
        // If less than 10 minutes since last board reset, let wifiManager do its thing then check for NFC tags and DON'T deep sleep the board
        m_wifiManager.process();

        m_server.handleClient();
        ElegantOTA.loop();

        checkDomeLight(m_whichCar);

        checkTrunkOpenCounter();

        if (debugSerialOn)
        {
            Serial.print("UNDER threshold time: ");
            Serial.println(m_espRtc.getLocalEpoch());

            Serial.print("trunkOpenCounter: ");
            Serial.println(m_trunkOpenCounter);
        }

        // This all works great, also tested reboot with switches set to on in case accidentally strapping pins, board still boots fine:

        // Serial.println();
        // Serial.println();
        // Serial.print("button01State: ");
        // Serial.println(button01State);
        //
        // Serial.print("switch02State: ");
        // Serial.println(switch02State);
        //
        // Serial.print("switch03State: ");
        // Serial.println(switch03State);

        m_pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, false);
    }
    else
    {
        // If more than 10 minutes since last board reset, then just check for tag then deep sleep
        checkDomeLight(m_whichCar);

        checkTrunkOpenCounter();

        if (debugSerialOn)
        {
            Serial.print("Over threshold time, should be no more AP: ");
            Serial.println(m_espRtc.getLocalEpoch());
        }

        m_pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, true);
    }
}

void checkDomeLight(const WhichCar whichCar)
{
    const unsigned long long localEpoch = m_espRtc.getLocalEpoch();

    const unsigned long long secondsSinceLastDoorOpen = localEpoch - m_epochAtLastDoorOpened;

    int domePinRead = 999;
    bool doorOpen = false;

    if (whichCar == WhichCar::HONDA_2008)
    {
        domePinRead = analogRead(Definitions::PIN_DOME_LIGHT);
        doorOpen = domePinRead < 460;
    }

    if (whichCar == WhichCar::COROLLA_2021)
    {
        domePinRead = analogRead(Definitions::PIN_DOME_LIGHT);
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

    if (!m_carLockedOnceFlag &&
        secondsSinceLastDoorOpen > 30)
    {
        m_carHelper.LockAllDoors();

        m_carLockedOnceFlag = true;
    }

    if (!m_carLockedTwiceFlag &&
        secondsSinceLastDoorOpen > 65)
    {
        m_carHelper.LockAllDoors();

        m_carLockedTwiceFlag = true;
    }

    if (doorOpen)
    {
        m_epochAtLastDoorOpened = m_espRtc.getLocalEpoch();

        m_carLockedOnceFlag = false;
        m_carLockedTwiceFlag = false;
    }
}

