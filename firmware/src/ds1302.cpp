#include "ds1302.h"

ThreeWire myWire(4, 2, 1); // IO, SCLK, CE
RtcDS1302<ThreeWire> Rtc(myWire);
struct tm timeinfo;

void printDateTime(const RtcDateTime &dt)
{
    char datestring[20];

    snprintf_P(datestring,
               countof(datestring),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u"),
               dt.Month(),
               dt.Day(),
               dt.Year(),
               dt.Hour(),
               dt.Minute(),
               dt.Second());
    Serial.print(datestring);
}

void rtc_init()
{
    Rtc.Begin();

    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__);
    printDateTime(compiled);
    Serial.println();

    if (!Rtc.IsDateTimeValid())
    {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected())
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning())
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }
}

void rtc_routine()
{
    RtcDateTime now = Rtc.GetDateTime();

    printDateTime(now);
    Serial.println();

    if (!now.IsValid())
    {
        // Common Causes:
        //    1) the battery on the device is low or even missing and the power line was disconnected
        Serial.println("RTC lost confidence in the DateTime!");
    }
}

// RtcDateTime(uint16_t year,
//     uint8_t month,
//     uint8_t dayOfMonth,
//     uint8_t hour,
//     uint8_t minute,
//     uint8_t second) :
//     _yearFrom2000((year >= c_OriginYear) ? year - c_OriginYear : year),
//     _month(month),
//     _dayOfMonth(dayOfMonth),
//     _hour(hour),
//     _minute(minute),
//     _second(second)

void rtc_sync_time()
{
    configTime(8 * 3600, 0, ntp_server);
    getLocalTime(&timeinfo);
    RtcDateTime sync_time(timeinfo.tm_year, timeinfo.tm_mon, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
    if (timeinfo.tm_year != 70)
    {
        Rtc.SetDateTime(sync_time);
    }
}