// 本地时钟模块，使用DS1302
// 在没有网络的情况下，提供时钟

#ifndef SHANG_RTC
#define SHANG_RTC

#include "conf.h"
#include <Arduino.h>
#include <ThreeWire.h>
#include <RtcDS1302.h>
#include <WiFi.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

// void printDateTime(const RtcDateTime &dt);
// DS1302初始化
void rtc_init();

#endif