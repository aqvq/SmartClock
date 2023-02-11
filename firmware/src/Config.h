#ifndef SHANG_CONFIG_H
#define SHANG_CONFIG_H
#include <Arduino.h>

#define wifi_ssid ("QAQ")
#define wifi_password ("802dot11")
#define ntp_server ("cn.pool.ntp.org")

constexpr uint16_t screen_height = 240;
constexpr uint16_t screen_width = 240;
constexpr uint16_t time_string_size = 10;
constexpr uint16_t screen_buffer_size = screen_height * screen_width / 10;

#endif