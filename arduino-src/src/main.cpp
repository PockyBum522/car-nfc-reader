#include <Arduino.h>
#include <Adafruit_PN532.h>
#include <Adafruit_SPIDevice.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include <ElegantOTA.h>
#include "Secrets/Secrets.h"
#include "Models/Definitions.h"
#include "Logic/SketchInitializers.h"
#include "Logic/CarHelper.h"
#include "Logic/EspEpochHelper.h"
#include "Logic/MainLoopDebugMessages.h"
#include "Logic/Pn532ShieldHandler.h"

bool m_debugSerialOn = true;
std::string m_versionMessage = "v02";

// Uncomment one:
//auto m_whichCar = WhichCar::HONDA_2008;
//auto m_whichCar = WhichCar::COROLLA_2003;
auto m_whichCar = WhichCar::COROLLA_2021;

// How long until it stops being able to get OTA updates after a reset
constexpr int m_wifiPortalTimeout = 1200;    // 1200s = 20 minutes

auto m_spiDevice = new Adafruit_SPIDevice(PN532_SS, PN532_SCK, PN532_MISO, PN532_MOSI,
    300000, SPI_BITORDER_LSBFIRST, SPI_MODE0);

Adafruit_PN532 m_nfcReader(PN532_SS, m_spiDevice);

auto m_epochHelper = *new EspEpochHelper();
auto m_sketchInitializers = *new SketchInitializers();
auto m_carHelper = *new CarHelper(m_whichCar, &m_epochHelper, m_debugSerialOn);
auto m_pn532ShieldHandler = *new Pn532ShieldHandler(&m_carHelper, &m_nfcReader, &m_epochHelper, m_debugSerialOn);
auto m_mainLoopDebugMessages = *new MainLoopDebugMessages(m_epochHelper, m_debugSerialOn, m_versionMessage, m_wifiPortalTimeout);
WiFiManager m_wifiManager;
WebServer m_server(80);

void setup()
{
    SketchInitializers::InitializeRgbLed(m_epochHelper.GetSecondsSinceBoardPowerOn(), m_wifiPortalTimeout);

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

    if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
        neopixelWrite(RGB_BUILTIN, 0, 0, 1);

    if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
    {
        // If less than timeout since last board reset, let wifiManager do its thing
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

    if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
        neopixelWrite(RGB_BUILTIN, 0, 1, 0);

    // // For power savings, set all pins not being used as inputs to outputs. It works.
    // for (int i = 0; i < 17; i++)
    // {
    //   if (i == 2){ i = 3; } // Skip ss pin
    //   if (i == 10){ i = 12; } // Skip lock/unlock pins
    //
    //   pinMode(i, OUTPUT);
    // }
}

void loop()
{
    if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
        neopixelWrite(RGB_BUILTIN, 1, 0, 1);

    m_carHelper.Loop();  // Handles checking for dome light, trunk open, and runs any queued actions

    m_mainLoopDebugMessages.Show();

    if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
    {
        // If less than 10 minutes since last board reset, let wifiManager do its thing then check for NFC tags and DON'T deep sleep the board
        m_wifiManager.process();

        m_server.handleClient();
        ElegantOTA.loop();

        neopixelWrite(RGB_BUILTIN, 0, 0, 0);

        m_pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, false);

        return;
    }

    m_pn532ShieldHandler.CheckForNfcTagAndPowerBackDown(Secrets::AuthorizedNfcTags, true, true);
}

