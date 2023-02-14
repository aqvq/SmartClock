#ifndef SHANG_PAGES_H
#define SHANG_PAGES_H

#include "conf.h"
#include <lvgl.h>
#include <WiFi.h>
#include "sensors.h"
#include "input.h"
#include "clock.h"

enum
{
    FORWARD = 1,
    BACKWARD,
    NOTSTART,
    WORKING,
    SUSPEND
};

void ui_count_reset();
void ui_count_inc();
void ui_count_dec();
void ui_time_timer100(lv_timer_t *timer);
void ui_time_timer150(lv_timer_t *timer);
void ui_time_timer500(lv_timer_t *timer);
void ui_time_timer700(lv_timer_t *timer);
void ui_clock_create_cancel_event(lv_event_t *event);
void ui_clock_list_delete_event(lv_event_t *event);
void ui_clock_item_init(lv_obj_t *parent, int hour, int min);
void ui_clock_create_confirm_event(lv_event_t *event);
void ui_clock_create_init(lv_obj_t *parent);
void ui_count_start_event(lv_event_t *event);
void ui_count_cancel_event(lv_event_t *event);
void ui_count_switch_event(lv_event_t *event);
void ui_count_init(lv_obj_t *parent);
void ui_clock_list_create_event(lv_event_t *event);
void ui_clock_list_init(lv_obj_t *parent);
void ui_time_init(lv_obj_t *parent);

#endif