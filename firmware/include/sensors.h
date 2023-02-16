// 温湿度传感器和蜂鸣器配置模块

#ifndef SHANG_DHT_H
#define SHANG_DHT_H

#include <DHT.h>
#define DHTPIN 17
#define DHTTYPE DHT11
#define BUZPIN 26
// 温湿度传感器初始化
void dht_init();
// 温湿度传感器更新参数
void dht_update(float *humidity, float *temperature);
// 蜂鸣器初始化
void buzzer_init();
// 蜂鸣器开始蜂鸣
void buzzer_start();
// 蜂鸣器停止蜂鸣
void buzzer_stop();
// 蜂鸣器蜂鸣指定时间
void buzzer_ring(uint16_t time);

#endif