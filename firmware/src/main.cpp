// 主模块

#include "conf.h"
#include "display.h"
#include "network.h"

void setup()
{
  dht_init();     // 传感器初始化
  buzzer_init();  // 蜂鸣器初始化
  display_init(); // 显示屏初始化
  network_init(); // 网络初始化
}

void loop()
{
  network_routine(); // 网络任务
  display_routine(); // 显示任务
}
