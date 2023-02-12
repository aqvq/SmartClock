#ifndef SHANG_NETWORK_H
#define SHANG_NETWORK_H

#include <Arduino.h>
#include <esp_wifi.h> //用于esp_wifi_restore() 删除保存的wifi信息
#include <WiFi.h>
#include "conf.h"

void wifi_init();
void wifi_close();
void wifi_smart_config();
bool wifi_auto_config();

#endif