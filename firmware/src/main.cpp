// 主模块

#include <Arduino.h>
#include "conf.h"
#include "display.h"
#include "network.h"
#include "ds1302.h"
#include <TaskScheduler.h>
#include "audio.h"

// Scheduler ts;
// Task task_network(0, TASK_FOREVER, &network_routine, &ts, true);
// Task task_display(0, TASK_FOREVER, &display_routine, &ts, true);

void setup()
{
  Serial.begin(115200);
  dht_init();   // 传感器初始化
  rtc_init();   // RTC初始化
  clock_init(); // 持久化数据初始化
  audio_init(); // 音频初始化
#if ENABLE_NETWORK
  network_init(); // 网络初始化
#endif
  display_init(); // 显示屏初始化
  Serial.println("SmartClock Starting...");
  // ts.startNow();
}

void loop()
{
#if ENABLE_NETWORK
  network_routine();
#endif
  display_routine();
  // ts.execute();
}
