#ifndef SHANG_CONFIG_H
#define SHANG_CONFIG_H
#include <Arduino.h>

#define ntp_server "cn.pool.ntp.org"
#define service_uuid "b408e1a0-3d8a-11ed-b878-0242ac120002"        // 服务UUID
#define characteristic_uuid "dfd84064-3d8a-11ed-b878-0242ac120002" // 特征UUID
#define control_uuid "de045162-3d97-11ed-b878-0242ac120002"        // 控制特征UUID
#define ble_name "SHANG_BLE"
#define blinker_auth "e8d111f79e67"

constexpr uint16_t screen_height = 240;
constexpr uint16_t screen_width = 240;
constexpr uint16_t time_string_size = 30;
constexpr uint16_t date_string_size = 50;
constexpr uint16_t humidity_string_size = 30;
constexpr uint16_t temperature_string_size = 30;
constexpr uint16_t screen_buffer_size = screen_height * screen_width / 10;
constexpr uint16_t key_long_press_time = 500;
#endif