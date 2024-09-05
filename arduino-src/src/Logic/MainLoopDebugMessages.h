#ifndef MAINLOOPDEBUGMESSAGES_H
#define MAINLOOPDEBUGMESSAGES_H


#include <Arduino.h>
#include <string>
#include <utility>

#include "EspEpochHelper.h"

class MainLoopDebugMessages
{
public:
    explicit MainLoopDebugMessages(const EspEpochHelper espEpochHelper, const bool debugSerialOn, std::string versionMessage, unsigned long long wifiPortalTimeout):
        m_debugSerialOn(debugSerialOn),
        m_versionMessage(std::move(versionMessage)),
        m_epochHelper(espEpochHelper),
        m_wifiPortalTimeout(wifiPortalTimeout) { }

    void Show()
    {
        if (!m_debugSerialOn) return;

        std::string thresholdUnderOver = "OVER";

        if (m_epochHelper.GetSecondsSinceBoardPowerOn() < m_wifiPortalTimeout)
            thresholdUnderOver = "UNDER";

        Serial.print("Ver: ");
        Serial.println(m_versionMessage.c_str());

        Serial.print("Seconds since board power on: ");
        Serial.print(m_epochHelper.GetSecondsSinceBoardPowerOn());
        Serial.print(" - Which is: ");
        Serial.print(thresholdUnderOver.c_str());
        Serial.println(" threshold time");

        Serial.print("Seconds since last read: ");
        Serial.println(m_epochHelper.GetSecondsSinceLastRead());
    }

private:
    EspEpochHelper m_epochHelper;

    bool m_debugSerialOn;

    const std::string m_versionMessage;

    unsigned long long m_wifiPortalTimeout;
};


#endif //MAINLOOPDEBUGMESSAGES_H
