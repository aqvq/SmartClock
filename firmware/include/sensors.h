// 温湿度传感器和蜂鸣器配置模块

#ifndef SHANG_DHT_H
#define SHANG_DHT_H

#include <DHT.h>
#define DHTPIN 25 // ???
#define DHTTYPE DHT11
#define BUZPIN 26
// 温湿度传感器初始化
void dht_init();
// 温湿度传感器更新参数
void dht_update(float *humidity, float *temperature);

#endif