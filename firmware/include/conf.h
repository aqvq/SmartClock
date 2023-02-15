// 参数设置文件

#ifndef SHANG_CONFIG_H
#define SHANG_CONFIG_H

#include <Arduino.h>
#include <lvgl.h>

// 宏定义
#define HIDDEN_SWITCH_WINDOW 1       // 通过隐藏的方式切换窗口
#define USE_DOUBLE_BUFFER 0          // 使用双缓冲区刷新屏幕
#define HOST_NAME "SmartClock"       // 设置设备名
#define AP_SSID "SmartClock"         // 设置AP热点名称
#define ntp_server "cn.pool.ntp.org" // ntp服务器地址
#define data_file "/data.txt"        // 闹钟数据存储的文件名

// 常量定义
constexpr uint16_t screen_height = 240;                                   // 屏幕高度
constexpr uint16_t screen_width = 240;                                    // 屏幕宽度
constexpr uint16_t time_string_size = 30;                                 // time字符串数组大小
constexpr uint16_t date_string_size = 50;                                 // date字符串数组大小
constexpr uint16_t humidity_string_size = 30;                             // 湿度字符串数组大小
constexpr uint16_t temperature_string_size = 30;                          // 温度字符串数组大小
constexpr uint16_t screen_buffer_size = screen_height * screen_width / 4; // 屏幕缓冲区大小
constexpr uint16_t max_clock_num = 24 * 60;                               // 最大闹钟数量，采用枚举法，共24*60个
constexpr uint16_t update_time_period = 1000 * 60 * 60 * 24;              // 网络校时周期
constexpr uint16_t connectTimeOut_s = 5;                                  // WiFi连接超时时间，单位秒

// 字体声明
LV_FONT_DECLARE(lv_chinese_20); // 中文20号微软雅黑字体
LV_FONT_DECLARE(lv_chinese_48); // 中文48号微软雅黑字体

#endif