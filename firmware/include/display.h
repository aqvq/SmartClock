#ifndef SHANG_DISPLAY_H
#define SHANG_DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "conf.h"
#include "sensors.h"
#include "input.h"
#include "ui.h"

void display_init();
void display_routine();

#endif