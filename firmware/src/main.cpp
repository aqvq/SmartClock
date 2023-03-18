// 主模块

#include <Arduino.h>
#include "conf.h"
#include "display.h"
#include "network.h"
#include "ds1302.h"
#include "audio.h"

void task_network(void *p);
void task_display(void *p);

void setup()
{
  Serial.begin(115200);
  dht_init();   // 传感器初始化
  rtc_init();   // RTC初始化
  clock_init(); // 持久化数据初始化
  audio_init(); // 音频初始化
#if ENABLE_NETWORK
  xTaskCreate(task_network, "network", 10000, NULL, 1, NULL);
#endif
  xTaskCreate(task_display, "display", 10000, NULL, 2, NULL);
  Serial.println("SmartClock Starting...");
}

void loop()
{
  vTaskDelay(1);
}

void task_network(void *p)
{
  network_init(); // 网络初始化
  while (1)
  {
    network_routine();
    vTaskDelay(1);
  }
}

void task_display(void *p)
{
  display_init(); // 显示屏初始化
  while (1)
  {
    display_routine();
    vTaskDelay(10);
  }
}
