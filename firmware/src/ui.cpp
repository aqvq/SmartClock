#include "ui.h"

static char time_string[time_string_size] = "加载中";
static char humidity_string[humidity_string_size] = "";
static char temperature_string[temperature_string_size] = "";
static char date_string[date_string_size] = "";
static const String WDAY_NAMES[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}; // 星期
static int last_second = -1;
static int last_minute = -1;
static int last_hour = -1;
float humidity = 0.0;
float temperature = 0.0;
struct tm timeinfo;

uint8_t direction = FORWARD;
uint8_t status = NOTSTART;

lv_obj_t *ui_Panel7;
lv_obj_t *ui_Button199;
lv_obj_t *ui_Panel1;
lv_obj_t *ui_Roller1;
lv_obj_t *ui_Label1;
lv_obj_t *ui_Roller2;
lv_obj_t *ui_Panel2;
lv_obj_t *ui_Button1;
lv_obj_t *ui_Label4;
lv_obj_t *ui_Button2;
lv_obj_t *ui_Label3;

lv_obj_t *ui_Panel23;
lv_obj_t *ui_Button99;
lv_obj_t *ui_Panel3;
lv_obj_t *ui_Roller4;
lv_obj_t *ui_Label5;
lv_obj_t *ui_Roller5;
lv_obj_t *ui_Label6;
lv_obj_t *ui_Roller6;
lv_obj_t *ui_Panel5;
lv_obj_t *ui_Label10;
lv_obj_t *ui_Switch1;
lv_obj_t *ui_Label11;
lv_obj_t *ui_Panel4;
lv_obj_t *ui_Button3;
lv_obj_t *ui_Label7;
lv_obj_t *ui_Button4;
lv_obj_t *ui_Label8;

lv_obj_t *ui_Panel6;
lv_obj_t *ui_Button6;
lv_obj_t *ui_Label12;
lv_obj_t *ui_Panel13;
lv_obj_t *ui_Label17;
lv_obj_t *ui_Button5;
lv_obj_t *ui_Label13;

lv_obj_t *ui_Label16;
lv_obj_t *ui_Label18;
lv_obj_t *ui_Label20;
lv_obj_t *ui_Panel10;
lv_obj_t *ui_Label19;
lv_obj_t *ui_Button9;

void ui_count_reset()
{
    lv_roller_set_selected(ui_Roller4, 0, LV_ANIM_ON);
    lv_roller_set_selected(ui_Roller5, 0, LV_ANIM_ON);
    lv_roller_set_selected(ui_Roller6, 0, LV_ANIM_ON);
    lv_label_set_text(ui_Label7, "开始");
}

void ui_count_inc()
{
    uint16_t cur_sec = lv_roller_get_selected(ui_Roller6);
    uint16_t cur_min = lv_roller_get_selected(ui_Roller5);
    uint16_t cur_hour = lv_roller_get_selected(ui_Roller4);
    if (cur_sec == 59)
    {
        if (cur_min == 59)
        {
            if (cur_hour == 23)
            {
                Serial.println("UI COUNT INC OVERFLOW!!!");
                status = NOTSTART;
                ui_count_reset();
            }
            else
            {
                lv_roller_set_selected(ui_Roller4, cur_hour + 1, LV_ANIM_ON);
                lv_roller_set_selected(ui_Roller5, 0, LV_ANIM_ON);
                lv_roller_set_selected(ui_Roller6, 0, LV_ANIM_ON);
            }
        }
        else
        {
            lv_roller_set_selected(ui_Roller5, cur_min + 1, LV_ANIM_ON);
            lv_roller_set_selected(ui_Roller6, 0, LV_ANIM_ON);
        }
    }
    else
    {
        lv_roller_set_selected(ui_Roller6, cur_sec + 1, LV_ANIM_ON);
    }
}

void ui_count_dec()
{
    uint16_t cur_sec = lv_roller_get_selected(ui_Roller6);
    uint16_t cur_min = lv_roller_get_selected(ui_Roller5);
    uint16_t cur_hour = lv_roller_get_selected(ui_Roller4);
    if (cur_sec == 00)
    {
        if (cur_min == 00)
        {
            if (cur_hour == 00)
            {
                Serial.println("UI COUNT DEC COMPLETE!!!");
                status = NOTSTART;
                ui_count_reset();
                buzzer_ring(100);
                delay(100);
                buzzer_ring(500);
            }
            else
            {
                lv_roller_set_selected(ui_Roller4, cur_hour - 1, LV_ANIM_ON);
                lv_roller_set_selected(ui_Roller6, 59, LV_ANIM_ON);
                lv_roller_set_selected(ui_Roller5, 59, LV_ANIM_ON);
            }
        }
        else
        {
            lv_roller_set_selected(ui_Roller5, cur_min - 1, LV_ANIM_ON);
            lv_roller_set_selected(ui_Roller6, 59, LV_ANIM_ON);
        }
    }
    else
    {
        lv_roller_set_selected(ui_Roller6, cur_sec - 1, LV_ANIM_ON);
    }
}

void ui_time_timer100(lv_timer_t *timer)
{
    lv_obj_t **children = (lv_obj_t **)timer->user_data;
    dht_update(&humidity, &temperature);
    getLocalTime(&timeinfo);
    if (WiFi.status() == WL_CONNECTED && timeinfo.tm_year != 70)
    {
        sprintf(time_string, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        sprintf(date_string, "%04d年%02d月%02d日 %s", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, WDAY_NAMES[timeinfo.tm_wday]);
        sprintf(humidity_string, "湿度: %2.1f%%", humidity);
        sprintf(temperature_string, "温度: %2.1f°C", temperature);
    }
    else
    {
        // sprintf(time_string, "__:__:__");
        // sprintf(date_string, "____年__月__日 星期__");
        // sprintf(humidity_string, "湿度: NaN");
        // sprintf(temperature_string, "温度: NaN");
        sprintf(time_string, "加载中");
        sprintf(date_string, "");
        sprintf(humidity_string, "");
        sprintf(temperature_string, "");
    }
    lv_label_set_text(children[0], time_string);
    lv_label_set_text(children[1], humidity_string);
    lv_label_set_text(children[2], temperature_string);
    lv_label_set_text(children[3], date_string);
}

void ui_time_timer120(lv_timer_t *timer)
{
    // 计时部分
    if ((status == WORKING) && (timeinfo.tm_sec != last_second || timeinfo.tm_min != last_minute || timeinfo.tm_hour != last_hour))
    {
        if (direction == FORWARD)
            ui_count_inc();
        else
            ui_count_dec();
    }
    last_second = timeinfo.tm_sec;
    last_minute = timeinfo.tm_min;
    last_hour = timeinfo.tm_hour;
}

void ui_time_timer500(lv_timer_t *timer)
{
    // 整点报时
    if (timeinfo.tm_sec == 0 && timeinfo.tm_min == 0)
    // if (timeinfo.tm_sec == 0)
    {
        buzzer_ring(100);
    }
}

void ui_time_timer700(lv_timer_t *timer)
{
    for (int i = 0; i < 24; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            if (clock_get(i, j) == true && timeinfo.tm_hour == i && timeinfo.tm_min == j && timeinfo.tm_sec < 3)
            {
                buzzer_ring(200);
            }
        }
    }
}

void ui_clock_create_cancel_event(lv_event_t *event)
{
#if HIDDEN_SWITCH_WINDOW
    lv_obj_add_flag(ui_Panel7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_HIDDEN);
#else
    ui_clock_list_init(ui_Panel7->parent);
    lv_obj_del(ui_Panel7);
#endif
}

void ui_clock_list_delete_event(lv_event_t *event)
{
    lv_obj_t *btn = lv_event_get_target(event);
    lv_obj_t *parent = btn->parent;
    lv_obj_t *label = lv_obj_get_child(parent, 0);
    int hour, min;
    sscanf(lv_label_get_text(label), "%d:%d", &hour, &min);
    clock_reset(hour, min);
    clock_save_all();
    lv_obj_del(parent);
}

void ui_clock_item_init(lv_obj_t *parent, int hour, int min)
{
    ui_Panel13 = lv_obj_create(parent);
    lv_obj_set_height(ui_Panel13, 60);
    lv_obj_set_width(ui_Panel13, lv_pct(100));
    lv_obj_set_x(ui_Panel13, 5);
    lv_obj_set_y(ui_Panel13, -59);
    lv_obj_set_align(ui_Panel13, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel13, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel13, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel13, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_pad_left(ui_Panel13, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel13, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel13, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    char tstring[time_string_size];
    sprintf(tstring, "%02d:%02d", hour, min);
    ui_Label17 = lv_label_create(ui_Panel13);
    lv_obj_set_width(ui_Label17, 60);
    lv_obj_set_height(ui_Label17, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label17, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label17, tstring);
    lv_obj_set_style_text_align(ui_Label17, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label17, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button5 = lv_btn_create(ui_Panel13);
    lv_obj_set_width(ui_Button5, 60);
    lv_obj_set_height(ui_Button5, 40);
    lv_obj_set_align(ui_Button5, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button5, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button5, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(ui_Button5, lv_color_hex(0xFF0000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label13 = lv_label_create(ui_Button5);
    lv_obj_set_width(ui_Label13, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label13, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label13, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label13, "删除");
    lv_obj_set_style_text_font(ui_Label13, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button5, ui_clock_list_delete_event, LV_EVENT_CLICKED, NULL);
}

void ui_clock_create_confirm_event(lv_event_t *event)
{
    uint16_t hour = lv_roller_get_selected(ui_Roller1);
    uint16_t min = lv_roller_get_selected(ui_Roller2);
    if (clock_get(hour, min) == false)
    {
        clock_set(hour, min);
        clock_save_all();
        ui_clock_item_init(ui_Panel6, hour, min);
    }

#if HIDDEN_SWITCH_WINDOW
    lv_obj_add_flag(ui_Panel7, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Panel6, LV_OBJ_FLAG_HIDDEN);
#else
    ui_clock_list_init(ui_Panel7->parent);
    lv_obj_del(ui_Panel7);
#endif
}

void ui_clock_create_init(lv_obj_t *parent)
{

    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(parent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel7 = lv_obj_create(parent);
    lv_obj_set_width(ui_Panel7, lv_pct(100));
    lv_obj_set_height(ui_Panel7, lv_pct(100));
    lv_obj_set_x(ui_Panel7, 2);
    lv_obj_set_y(ui_Panel7, 0);
    lv_obj_set_align(ui_Panel7, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel7, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel7, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel7, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel7, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_add_flag(ui_Panel7, LV_OBJ_FLAG_HIDDEN);

    ui_Button199 = lv_btn_create(ui_Panel7);
    lv_obj_set_width(ui_Button199, 0);
    lv_obj_set_height(ui_Button199, 0);
    lv_obj_set_align(ui_Button199, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_Button199, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button199, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(ui_Button199, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button199, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_Panel1 = lv_obj_create(ui_Panel7);
    lv_obj_set_height(ui_Panel1, 130);
    lv_obj_set_width(ui_Panel1, lv_pct(100));
    lv_obj_set_align(ui_Panel1, LV_ALIGN_TOP_MID);
    lv_obj_set_flex_flow(ui_Panel1, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel1, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller1 = lv_roller_create(ui_Panel1);
    lv_roller_set_options(ui_Roller1,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_Roller1, 120);
    lv_obj_set_width(ui_Roller1, lv_pct(25));
    lv_obj_set_x(ui_Roller1, 67);
    lv_obj_set_y(ui_Roller1, -1);
    lv_obj_set_align(ui_Roller1, LV_ALIGN_CENTER);

    ui_Label1 = lv_label_create(ui_Panel1);
    lv_obj_set_width(ui_Label1, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label1, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label1, -27);
    lv_obj_set_y(ui_Label1, -4);
    lv_obj_set_align(ui_Label1, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1, ":");

    ui_Roller2 = lv_roller_create(ui_Panel1);
    lv_roller_set_options(ui_Roller2,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_Roller2, 120);
    lv_obj_set_width(ui_Roller2, lv_pct(25));
    lv_obj_set_x(ui_Roller2, -4);
    lv_obj_set_y(ui_Roller2, -3);
    lv_obj_set_align(ui_Roller2, LV_ALIGN_CENTER);

    ui_Panel2 = lv_obj_create(ui_Panel7);
    lv_obj_set_height(ui_Panel2, 44);
    lv_obj_set_width(ui_Panel2, lv_pct(100));
    lv_obj_set_align(ui_Panel2, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_flex_flow(ui_Panel2, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel2, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button1 = lv_btn_create(ui_Panel2);
    lv_obj_set_width(ui_Button1, 80);
    lv_obj_set_height(ui_Button1, 30);
    lv_obj_set_align(ui_Button1, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button1, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button1, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_Label4 = lv_label_create(ui_Button1);
    lv_obj_set_width(ui_Label4, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label4, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label4, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4, "确认");
    lv_obj_set_style_text_font(ui_Label4, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button2 = lv_btn_create(ui_Panel2);
    lv_obj_set_width(ui_Button2, 80);
    lv_obj_set_height(ui_Button2, 30);
    lv_obj_set_align(ui_Button2, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button2, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button2, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_Label3 = lv_label_create(ui_Button2);
    lv_obj_set_width(ui_Label3, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label3, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label3, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3, "取消");
    lv_obj_set_style_text_font(ui_Label3, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Button1, ui_clock_create_confirm_event, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(ui_Button2, ui_clock_create_cancel_event, LV_EVENT_CLICKED, NULL);
}

void ui_count_start_event(lv_event_t *event)
{
    if (strcmp(lv_label_get_text(ui_Label7), "开始") == 0)
    {
        status = WORKING;
        lv_label_set_text(ui_Label7, "暂停");
    }
    else if (strcmp(lv_label_get_text(ui_Label7), "暂停") == 0)
    {
        status = SUSPEND;
        lv_label_set_text(ui_Label7, "继续");
    }
    else if (strcmp(lv_label_get_text(ui_Label7), "继续") == 0)
    {
        status = WORKING;
        lv_label_set_text(ui_Label7, "暂停");
    }
}

void ui_count_cancel_event(lv_event_t *event)
{
    status = NOTSTART;
    ui_count_reset();
    lv_label_set_text(ui_Label7, "开始");
}

void ui_count_switch_event(lv_event_t *event)
{
    if (lv_obj_has_state(lv_event_get_target(event), LV_STATE_CHECKED))
    {
        direction = BACKWARD;
    }
    else
    {
        direction = FORWARD;
    }
}

void ui_count_init(lv_obj_t *parent)
{

    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(parent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel23 = lv_obj_create(parent);
    lv_obj_set_width(ui_Panel23, lv_pct(100));
    lv_obj_set_height(ui_Panel23, lv_pct(100));
    lv_obj_set_align(ui_Panel23, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel23, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel23, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel23, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel23, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel23, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel23, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button99 = lv_btn_create(ui_Panel23);
    lv_obj_set_width(ui_Button99, 0);
    lv_obj_set_height(ui_Button99, 0);
    lv_obj_set_align(ui_Button99, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_Button99, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button99, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(ui_Button99, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button99, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    ui_Panel3 = lv_obj_create(ui_Panel23);
    lv_obj_set_height(ui_Panel3, 60);
    lv_obj_set_width(ui_Panel3, lv_pct(98));
    lv_obj_set_x(ui_Panel3, -36);
    lv_obj_set_y(ui_Panel3, -76);
    lv_obj_set_align(ui_Panel3, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel3, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel3, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel3, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller4 = lv_roller_create(ui_Panel3);
    lv_roller_set_options(ui_Roller4,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_Roller4, 50);
    lv_obj_set_width(ui_Roller4, lv_pct(25));
    lv_obj_set_x(ui_Roller4, 67);
    lv_obj_set_y(ui_Roller4, -1);
    lv_obj_set_align(ui_Roller4, LV_ALIGN_CENTER);
    // lv_obj_clear_flag(ui_Roller4, LV_OBJ_FLAG_CLICKABLE);

    ui_Label5 = lv_label_create(ui_Panel3);
    lv_obj_set_width(ui_Label5, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label5, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label5, -27);
    lv_obj_set_y(ui_Label5, -4);
    lv_obj_set_align(ui_Label5, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5, ":");

    ui_Roller5 = lv_roller_create(ui_Panel3);
    lv_roller_set_options(ui_Roller5,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_Roller5, 50);
    lv_obj_set_width(ui_Roller5, lv_pct(25));
    lv_obj_set_x(ui_Roller5, -4);
    lv_obj_set_y(ui_Roller5, -3);
    lv_obj_set_align(ui_Roller5, LV_ALIGN_CENTER);
    // lv_obj_clear_flag(ui_Roller5, LV_OBJ_FLAG_CLICKABLE);

    ui_Label6 = lv_label_create(ui_Panel3);
    lv_obj_set_width(ui_Label6, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label6, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label6, -27);
    lv_obj_set_y(ui_Label6, -4);
    lv_obj_set_align(ui_Label6, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6, ":");

    ui_Roller6 = lv_roller_create(ui_Panel3);
    lv_roller_set_options(ui_Roller6,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_height(ui_Roller6, 50);
    lv_obj_set_width(ui_Roller6, lv_pct(25));
    lv_obj_set_x(ui_Roller6, -60);
    lv_obj_set_y(ui_Roller6, -3);
    lv_obj_set_align(ui_Roller6, LV_ALIGN_CENTER);
    // lv_obj_clear_flag(ui_Roller6, LV_OBJ_FLAG_CLICKABLE);

    ui_Panel5 = lv_obj_create(ui_Panel23);
    lv_obj_set_height(ui_Panel5, 50);
    lv_obj_set_width(ui_Panel5, lv_pct(100));
    lv_obj_set_align(ui_Panel5, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel5, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel5, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel5, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel5, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel5, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label10 = lv_label_create(ui_Panel5);
    lv_obj_set_width(ui_Label10, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label10, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label10, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label10, "正计时");
    lv_obj_set_style_text_font(ui_Label10, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Switch1 = lv_switch_create(ui_Panel5);
    lv_obj_set_width(ui_Switch1, 50);
    lv_obj_set_height(ui_Switch1, 25);
    lv_obj_set_align(ui_Switch1, LV_ALIGN_CENTER);

    ui_Label11 = lv_label_create(ui_Panel5);
    lv_obj_set_width(ui_Label11, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label11, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label11, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label11, "倒计时");
    lv_obj_set_style_text_font(ui_Label11, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel4 = lv_obj_create(ui_Panel23);
    lv_obj_set_height(ui_Panel4, 50);
    lv_obj_set_width(ui_Panel4, lv_pct(100));
    lv_obj_set_x(ui_Panel4, -17);
    lv_obj_set_y(ui_Panel4, 68);
    lv_obj_set_align(ui_Panel4, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel4, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel4, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel4, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel4, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel4, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel4, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button3 = lv_btn_create(ui_Panel4);
    lv_obj_set_width(ui_Button3, 80);
    lv_obj_set_height(ui_Button3, 36);
    lv_obj_set_align(ui_Button3, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button3, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button3, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_Label7 = lv_label_create(ui_Button3);
    lv_obj_set_width(ui_Label7, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label7, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label7, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7, "开始");
    lv_obj_set_style_text_font(ui_Label7, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button4 = lv_btn_create(ui_Panel4);
    lv_obj_set_width(ui_Button4, 80);
    lv_obj_set_height(ui_Button4, 36);
    lv_obj_set_align(ui_Button4, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button4, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button4, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_Label8 = lv_label_create(ui_Button4);
    lv_obj_set_width(ui_Label8, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label8, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label8, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8, "重置");
    lv_obj_set_style_text_font(ui_Label8, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_add_event_cb(ui_Switch1, ui_count_switch_event, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Button3, ui_count_start_event, LV_EVENT_CLICKED, NULL);  /*Assign an event callback*/
    lv_obj_add_event_cb(ui_Button4, ui_count_cancel_event, LV_EVENT_CLICKED, NULL); /*Assign an event callback*/
}

void ui_clock_list_create_event(lv_event_t *event)
{
#if HIDDEN_SWITCH_WINDOW
    lv_obj_add_flag(ui_Panel6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Panel7, LV_OBJ_FLAG_HIDDEN);
#else
    ui_clock_create_init(ui_Panel6->parent);
    lv_obj_del(ui_Panel6);
#endif
    lv_roller_set_selected(ui_Roller1, timeinfo.tm_hour, LV_ANIM_ON);
    lv_roller_set_selected(ui_Roller2, timeinfo.tm_min, LV_ANIM_ON);
}

void ui_clock_list_init(lv_obj_t *parent)
{
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE); /// Flags

    ui_Panel6 = lv_obj_create(parent);
    lv_obj_set_width(ui_Panel6, lv_pct(100));
    lv_obj_set_height(ui_Panel6, lv_pct(100));
    lv_obj_set_align(ui_Panel6, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel6, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel6, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_border_color(ui_Panel6, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel6, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel6, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel6, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel6, 10, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button6 = lv_btn_create(ui_Panel6);
    lv_obj_set_height(ui_Button6, 60);
    lv_obj_set_width(ui_Button6, lv_pct(100));
    lv_obj_set_align(ui_Button6, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Button6, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button6, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    ui_Label12 = lv_label_create(ui_Button6);
    lv_obj_set_width(ui_Label12, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label12, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label12, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label12, "添加");
    lv_obj_set_style_text_align(ui_Label12, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_Label12, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    clock_init();
    clock_read_all();
    for (int i = 0; i < 24; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            if (clock_get(i, j) == true)
            {
                ui_clock_item_init(ui_Panel6, i, j);
            }
        }
    }

    lv_obj_add_event_cb(ui_Button6, ui_clock_list_create_event, LV_EVENT_CLICKED, NULL);
}

void ui_time_init(lv_obj_t *parent)
{

    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(parent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label16 = lv_label_create(parent);
    lv_obj_set_width(ui_Label16, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label16, LV_SIZE_CONTENT); /// 1
    lv_label_set_text(ui_Label16, "");
    lv_obj_set_style_text_font(ui_Label16, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label18 = lv_label_create(parent);
    lv_obj_set_width(ui_Label18, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label18, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label18, LV_ALIGN_TOP_RIGHT);
    lv_label_set_text(ui_Label18, "");
    lv_obj_set_style_text_font(ui_Label18, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label20 = lv_label_create(parent);
    lv_obj_set_width(ui_Label20, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label20, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label20, LV_ALIGN_BOTTOM_MID);
    lv_label_set_text(ui_Label20, "");
    lv_obj_set_style_text_font(ui_Label20, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel10 = lv_obj_create(parent);
    lv_obj_set_height(ui_Panel10, 70);
    lv_obj_set_width(ui_Panel10, lv_pct(100));
    lv_obj_set_align(ui_Panel10, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel10, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel10, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel10, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel10, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel10, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label19 = lv_label_create(ui_Panel10);
    lv_obj_set_width(ui_Label19, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label19, LV_SIZE_CONTENT); /// 1
    lv_obj_set_align(ui_Label19, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label19, "加载中");
    lv_obj_set_style_text_font(ui_Label19, &lv_chinese_48, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Button9 = lv_btn_create(ui_Panel10);
    lv_obj_set_width(ui_Button9, 0);
    lv_obj_set_height(ui_Button9, 0);
    lv_obj_set_align(ui_Button9, LV_ALIGN_BOTTOM_MID);
    lv_obj_add_flag(ui_Button9, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Button9, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_shadow_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_border_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_outline_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_shadow_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_PRESSED);
    lv_obj_set_style_bg_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_bg_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_border_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_outline_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_color(ui_Button9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_FOCUSED);
    lv_obj_set_style_shadow_opa(ui_Button9, 255, LV_PART_MAIN | LV_STATE_FOCUSED);

    static lv_obj_t *ui_time_obj_children[4] = {ui_Label19, ui_Label18, ui_Label16, ui_Label20};
    lv_timer_t *timer1 = lv_timer_create(ui_time_timer100, 100, ui_time_obj_children);
    lv_timer_t *timer2 = lv_timer_create(ui_time_timer120, 120, NULL);
    lv_timer_t *timer3 = lv_timer_create(ui_time_timer500, 500, NULL);
    lv_timer_t *timer4 = lv_timer_create(ui_time_timer700, 700, NULL);
}
