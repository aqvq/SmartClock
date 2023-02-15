// UI渲染与控制逻辑模块
// 是本项目的核心模块

#ifndef SHANG_PAGES_H
#define SHANG_PAGES_H

#include "conf.h"
#include <lvgl.h>
#include <WiFi.h>
#include "sensors.h"
#include "input.h"
#include "clock.h"

// 定义状态常量
enum
{
    FORWARD = 1,
    BACKWARD,
    NOTSTART,
    WORKING,
    SUSPEND
};

// 重置定时器
void ui_count_reset();
// 定时器正计时逻辑
void ui_count_inc();
// 定时器倒计时逻辑
void ui_count_dec();
// 温湿度、时间、日期更新逻辑
void ui_time_timer100(lv_timer_t *timer);
// 定时器计时逻辑
void ui_time_timer120(lv_timer_t *timer);
// 整点报时逻辑
void ui_time_timer500(lv_timer_t *timer);
// 闹钟响铃逻辑
void ui_time_timer700(lv_timer_t *timer);
// 创建闹钟页面按下取消按钮事件
void ui_clock_create_cancel_event(lv_event_t *event);
// 闹钟列表页面按下删除按钮
void ui_clock_list_delete_event(lv_event_t *event);
// 初始化闹钟列表的一个子项
void ui_clock_item_init(lv_obj_t *parent, int hour, int min);
// 创建闹钟页面按下确认按钮时间
void ui_clock_create_confirm_event(lv_event_t *event);
// 创建闹钟页面初始化
void ui_clock_create_init(lv_obj_t *parent);
// 定时器按下开始事件
void ui_count_start_event(lv_event_t *event);
// 定时器按下取消事件
void ui_count_cancel_event(lv_event_t *event);
// 切换正计时与倒计时
void ui_count_switch_event(lv_event_t *event);
// 定时器页面初始化
void ui_count_init(lv_obj_t *parent);
// 闹钟列表页面按下创建按钮事件
void ui_clock_list_create_event(lv_event_t *event);
// 闹钟列表页面初始化
void ui_clock_list_init(lv_obj_t *parent);
// 时间首页初始化
void ui_time_init(lv_obj_t *parent);

#endif