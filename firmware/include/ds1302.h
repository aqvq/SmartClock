#ifndef SHANG_RTC
#define SHANG_RTC

#include "conf.h"
#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

void printDateTime(const RtcDateTime &dt);
void rtc_init();
void rtc_routine();
void rtc_sync_time();

#endif