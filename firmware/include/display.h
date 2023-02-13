#ifndef SHANG_DISPLAY_H
#define SHANG_DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "conf.h"
#include "sensors.h"
// #include "page.h"
#include "input.h"
LV_FONT_DECLARE(lv_chinese_20);
LV_FONT_DECLARE(lv_chinese_48);
void display_init();
void display_routine();

#endif