#ifndef ESPEPOCHHELPER_H
#define ESPEPOCHHELPER_H


#include <ESP32Time.h>

class EspEpochHelper
{
public:
    explicit EspEpochHelper()
    {
        m_espRtc = ESP32Time(0);
    }

    // Board power on
    unsigned long long GetSecondsSinceBoardPowerOn()
    {
        return m_espRtc.getLocalEpoch();         // now: timestamp
    }

    void ResetSecondsSinceBoardPowerOn()
    {
        m_espRtc.setTime(0);
    }

    unsigned long long GetMillisecondsSinceBoardPowerOn()
    {
        return m_espRtc.getMillis();
    }

    // Last NFC read time
    unsigned long long GetSecondsSinceLastRead()
    {
        return m_espRtc.getLocalEpoch() - m_lastReadAt;
    }

    void SetLastReadToNow()
    {
        m_lastReadAt = m_espRtc.getLocalEpoch();
    }

    // Last door opened at time
    unsigned long long GetSecondsSinceDoorLastOpen()
    {
        return m_espRtc.getLocalEpoch() - m_lastDoorOpenAt;
    }

    void SetDoorLastOpenAtToNow()
    {
        m_lastDoorOpenAt = m_espRtc.getLocalEpoch();
    }

private:
    ESP32Time m_espRtc;

    static unsigned long long m_lastReadAt;
    static unsigned long long m_lastDoorOpenAt;
};

unsigned long long EspEpochHelper::m_lastReadAt = 0;
unsigned long long EspEpochHelper::m_lastDoorOpenAt = 0;


#endif //ESPEPOCHHELPER_H
