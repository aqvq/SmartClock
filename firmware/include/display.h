// LVGL显示驱动模块

#ifndef SHANG_DISPLAY_H
#define SHANG_DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "conf.h"
#include "sensors.h"
#include "input.h"
#include "ui.h"

// 初始化显示驱动
void display_init();
// LVGL日常任务执行
void display_routine();

#endif