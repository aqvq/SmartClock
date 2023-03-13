// 主模块

#include <Arduino.h>
#include "conf.h"
#include "display.h"
#include "network.h"
#include "ds1302.h"
#include <TaskScheduler.h>

Scheduler ts;
Task task_network(0, TASK_FOREVER, &network_routine, &ts, true);
Task task_display(0, TASK_FOREVER, &display_routine, &ts, true);
void setup()
{
  Serial.begin(115200);
  dht_init();     // 传感器初始化
  buzzer_init();  // 蜂鸣器初始化
  rtc_init();     // RTC初始化
  clock_init();   // 持久化数据初始化
  network_init(); // 网络初始化
  display_init(); // 显示屏初始化
  Serial.println("Start");
  ts.startNow();
}

void loop()
{
  ts.execute();
}