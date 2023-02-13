#ifndef LV_PORT_INDEV_TEMPL_H
#define LV_PORT_INDEV_TEMPL_H

#include <lvgl.h>
#include "conf.h"
#include <ESP32Encoder.h>

#define ENCODER_A 21
#define ENCODER_C 18
#define ENCODER_B 19

void input_init(void);
static void encoder_init(void);
static void encoder_read(lv_indev_drv_t *indev_drv, lv_indev_data_t *data);
static void encoder_handler(void);

#endif
