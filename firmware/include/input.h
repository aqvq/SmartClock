// LVGL输入驱动模块

#ifndef SHANG_INPUT_H
#define SHANG_INPUT_H

#include <lvgl.h>
#include "conf.h"
#include <ESP32Encoder.h>

// 管脚定义
#define ENCODER_A 21
#define ENCODER_C 18
#define ENCODER_B 19

// 输入驱动初始化
void input_init(void);
// 旋转编码器初始化
static void encoder_init(void);
// 旋转编码器读取状态函数，由LVGL系统调用
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
// 暂未使用
static void encoder_handler(void);

#endif
