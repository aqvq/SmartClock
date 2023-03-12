// 主模块

#include "conf.h"
#include "display.h"
#include "network.h"
#include "ds1302.h"

void setup()
{
  Serial.begin(115200);
  rtc_init();
  dht_init();     // 传感器初始化
  buzzer_init();  // 蜂鸣器初始化
  display_init(); // 显示屏初始化
  network_init(); // 网络初始化
}

void loop()
{
  rtc_routine();
  display_routine(); // 显示任务
  network_routine(); // 网络任务
}
