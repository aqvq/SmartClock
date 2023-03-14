// 此模块是项目功能逻辑代码，其他文件都是驱动代码
// 此模块先使用SquareLine进行基本代码的生成
// 但是SquareLine使用限制并且并不十分好用
// 电脑预览和实际显示并不完全相同，需要手动微调
// 本模块界面渲染与逻辑控制深度耦合，不易扩展
// 但是采用TabView，比较好在页面层级上进行扩展

#include "ui.h"

char time_string[time_string_size] = "连接网络";
char date_string[date_string_size] = "";
static char humidity_string[humidity_string_size] = "";
static char temperature_string[temperature_string_size] = "";
static const String WDAY_NAMES[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}; // 星期
static int last_second = -1;
static int last_minute = -1;
static int last_hour = -1;
float humidity = 0.0;
float temperature = 0.0;
int time_status;
unsigned long initialized_time;
uint8_t direction = FORWARD;
uint8_t status = NOTSTART;
extern RtcDS1302<ThreeWire> Rtc;
RtcDateTime now;
extern StoredConfig stored_config;
extern bool first_sync;
extern unsigned long sync_time;
extern DFRobotDFPlayerMini myDFPlayer;

lv_obj_t *ui_Panel7;
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

lv_obj_t *ui_Panel3131;
lv_obj_t *ui_Panel2131;
lv_obj_t *ui_Roller2131;
lv_obj_t *ui_Label1131;
lv_obj_t *ui_Roller7131;
lv_obj_t *ui_Label2131;
lv_obj_t *ui_Roller8131;
lv_obj_t *ui_Label3131;
lv_obj_t *ui_Panel6131;
lv_obj_t *ui_Roller1131;
lv_obj_t *ui_Label4131;
lv_obj_t *ui_Roller3131;
lv_obj_t *ui_Label7131;
lv_obj_t *ui_Roller9131;
lv_obj_t *ui_Label8131;
lv_obj_t *ui_Panel5131;
lv_obj_t *ui_Label5131;
lv_obj_t *ui_Slider3131;
lv_obj_t *ui_Panel1131;
lv_obj_t *ui_Label6131;
lv_obj_t *ui_Slider2131;

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
                myDFPlayer.playFolder(1, 1);
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
    // getLocalTime(&timeinfo);
    now = Rtc.GetDateTime();
    // if (WiFi.status() == WL_CONNECTED)
    // {
    if (now.IsValid())
    {
        if (WiFi.status() != WL_CONNECTED || sync_time != 0)
        {
            if (time_status != NORMAL)
                initialized_time = millis();
            time_status = NORMAL;
            sprintf(time_string, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
            sprintf(date_string, "%04d年%02d月%02d日 %s", now.Year(), now.Month(), now.Day(), WDAY_NAMES[now.DayOfWeek()]);
            sprintf(humidity_string, "湿度: %2.0f%%", humidity);
            sprintf(temperature_string, "温度: %2.0f°C", temperature);
        }
        else
        {
            sprintf(time_string, "连接网络");
            sprintf(date_string, "");
            sprintf(humidity_string, "");
            sprintf(temperature_string, "");
        }
    }
    else
    {
        sprintf(time_string, "时间错误");
        sprintf(date_string, "");
        sprintf(humidity_string, "");
        sprintf(temperature_string, "");
    }

    // if (timeinfo.tm_year != 70)
    // {
    //     if (time_status != NORMAL)
    //         initialized_time = millis();
    //     time_status = NORMAL;
    //     sprintf(time_string, "%02d:%02d:%02d", now.Hour(), now.Minute(), now.Second());
    //     sprintf(date_string, "%04d年%02d月%02d日 %s", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, WDAY_NAMES[timeinfo.tm_wday]);
    //     sprintf(humidity_string, "湿度: %2.1f%%", humidity);
    //     sprintf(temperature_string, "温度: %2.1f°C", temperature);
    // }
    // else
    // {
    //     if (time_status != SETTIME)
    //     {
    //         sprintf(time_string, "同步时间");
    //         sprintf(date_string, "");
    //         sprintf(humidity_string, "");
    //         sprintf(temperature_string, "");
    //         configTime(8 * 3600, 0, ntp_server);
    //         time_status = SETTIME;
    //     }
    // }
    // }
    // else
    // {
    //     if (time_status != NONET)
    //     {
    //         sprintf(time_string, "连接网络");
    //         sprintf(date_string, "");
    //         sprintf(humidity_string, "");
    //         sprintf(temperature_string, "");
    //         time_status = NONET;
    //     }
    // }
    lv_label_set_text(children[0], time_string);
    lv_label_set_text(children[1], humidity_string);
    lv_label_set_text(children[2], temperature_string);
    lv_label_set_text(children[3], date_string);
}

void ui_time_timer120(lv_timer_t *timer)
{
    // 计时部分
    now = Rtc.GetDateTime();
    if ((status == WORKING) && (now.Second() != last_second || now.Minute() != last_minute || now.Hour() != last_hour))
    {
        if (direction == FORWARD)
            ui_count_inc();
        else
            ui_count_dec();
    }
    last_second = now.Second();
    last_minute = now.Minute();
    last_hour = now.Hour();
}

void ui_time_timer800(lv_timer_t *timer)
{
    now = Rtc.GetDateTime();
    // 闹铃功能
    for (int i = 0; i < 24; ++i)
    {
        for (int j = 0; j < 60; ++j)
        {
            if (clock_get(i, j) == true && now.Hour() == i && now.Minute() == j && now.Second() == 0)
            {
                myDFPlayer.playFolder(1, 2);
            }
        }
    }

    // 整点报时
    if (now.Second() == 0 && now.Minute() == 0)
    // if (now.Second() == 0)
    {
        myDFPlayer.advertise(now.Hour());
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
    lv_obj_add_flag(ui_Roller1, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller1, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

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
    lv_obj_add_flag(ui_Roller2, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller2, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

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
    lv_obj_add_flag(ui_Roller4, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller4, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

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
    lv_obj_add_flag(ui_Roller5, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller5, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

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
    lv_obj_add_flag(ui_Roller6, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller6, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

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
    lv_roller_set_selected(ui_Roller1, now.Hour(), LV_ANIM_ON);
    lv_roller_set_selected(ui_Roller2, now.Minute(), LV_ANIM_ON);
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
    lv_obj_add_event_cb(ui_Button9, ui_time_sync_time_cb, LV_EVENT_CLICKED, NULL);

    static lv_obj_t *ui_time_obj_children[4] = {ui_Label19, ui_Label18, ui_Label16, ui_Label20};
    lv_timer_t *timer1 = lv_timer_create(ui_time_timer100, 100, ui_time_obj_children);
    lv_timer_t *timer2 = lv_timer_create(ui_time_timer120, 120, NULL);
    lv_timer_t *timer4 = lv_timer_create(ui_time_timer800, 800, NULL);
}

void ui_time_sync_time_cb(lv_event_t *e)
{
    first_sync = false;
}

void ui_settings_init(lv_obj_t *parent)
{
    lv_obj_clear_flag(parent, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_bg_color(parent, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(parent, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(parent, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel3131 = lv_obj_create(parent);
    lv_obj_set_width(ui_Panel3131, lv_pct(100));
    lv_obj_set_height(ui_Panel3131, lv_pct(100));
    lv_obj_set_x(ui_Panel3131, 0);
    lv_obj_set_y(ui_Panel3131, -2);
    lv_obj_set_align(ui_Panel3131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel3131, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel3131, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel3131, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel3131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel2131, 45);
    lv_obj_set_width(ui_Panel2131, lv_pct(100));
    lv_obj_set_x(ui_Panel2131, 3);
    lv_obj_set_y(ui_Panel2131, -78);
    lv_obj_set_align(ui_Panel2131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel2131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel2131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2131, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel2131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller2131 = lv_roller_create(ui_Panel2131);
    // lv_roller_set_options(ui_Roller2131, "2021\n2022\n2023\n2024\n2025", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller2131, 55);
    lv_obj_set_height(ui_Roller2131, 30);
    lv_obj_set_align(ui_Roller2131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller2131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller2131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller2131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller2131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller2131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller2131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label1131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label1131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label1131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label1131, -17);
    lv_obj_set_y(ui_Label1131, -8);
    lv_obj_set_align(ui_Label1131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1131, "年");
    lv_obj_set_style_text_font(ui_Label1131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller7131 = lv_roller_create(ui_Panel2131);
    lv_roller_set_options(ui_Roller7131,
                          "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller7131, 35);
    lv_obj_set_height(ui_Roller7131, 30);
    lv_obj_set_align(ui_Roller7131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller7131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller7131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller7131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller7131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller7131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller7131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label2131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label2131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label2131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label2131, 2);
    lv_obj_set_y(ui_Label2131, -56);
    lv_obj_set_align(ui_Label2131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2131, "月");
    lv_obj_set_style_text_font(ui_Label2131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller8131 = lv_roller_create(ui_Panel2131);
    lv_roller_set_options(ui_Roller8131,
                          "01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller8131, 35);
    lv_obj_set_height(ui_Roller8131, 30);
    lv_obj_set_align(ui_Roller8131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller8131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller8131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller8131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller8131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller8131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller8131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label3131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label3131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label3131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label3131, 63);
    lv_obj_set_y(ui_Label3131, -53);
    lv_obj_set_align(ui_Label3131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3131, "日");
    lv_obj_set_style_text_font(ui_Label3131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel6131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel6131, 45);
    lv_obj_set_width(ui_Panel6131, lv_pct(100));
    lv_obj_set_x(ui_Panel6131, 3);
    lv_obj_set_y(ui_Panel6131, -78);
    lv_obj_set_align(ui_Panel6131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel6131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel6131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6131, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel6131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller1131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller1131,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller1131, 35);
    lv_obj_set_height(ui_Roller1131, 30);
    lv_obj_set_align(ui_Roller1131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller1131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller1131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller1131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller1131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller1131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller1131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label4131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label4131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label4131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label4131, -17);
    lv_obj_set_y(ui_Label4131, -8);
    lv_obj_set_align(ui_Label4131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4131, "时");
    lv_obj_set_style_text_font(ui_Label4131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller3131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller3131,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller3131, 35);
    lv_obj_set_height(ui_Roller3131, 30);
    lv_obj_set_align(ui_Roller3131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller3131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller3131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller3131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller3131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller3131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller3131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label7131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label7131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label7131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label7131, 2);
    lv_obj_set_y(ui_Label7131, -56);
    lv_obj_set_align(ui_Label7131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7131, "分");
    lv_obj_set_style_text_font(ui_Label7131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller9131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller9131,
                          "00\n01\n02\n03\n04\n05\n06\n07\n08\n09\n10\n11\n12\n13\n14\n15\n16\n17\n18\n19\n20\n21\n22\n23\n24\n25\n26\n27\n28\n29\n30\n31\n32\n33\n34\n35\n36\n37\n38\n39\n40\n41\n42\n43\n44\n45\n46\n47\n48\n49\n50\n51\n52\n53\n54\n55\n56\n57\n58\n59",
                          LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller9131, 35);
    lv_obj_set_height(ui_Roller9131, 30);
    lv_obj_set_align(ui_Roller9131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Roller9131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Roller9131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags

    lv_obj_set_style_text_color(ui_Roller9131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller9131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller9131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller9131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label8131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label8131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label8131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label8131, 63);
    lv_obj_set_y(ui_Label8131, -53);
    lv_obj_set_align(ui_Label8131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8131, "秒");
    lv_obj_set_style_text_font(ui_Label8131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel5131, 35);
    lv_obj_set_width(ui_Panel5131, lv_pct(100));
    lv_obj_set_x(ui_Panel5131, 0);
    lv_obj_set_y(ui_Panel5131, 81);
    lv_obj_set_align(ui_Panel5131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel5131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel5131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel5131, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel5131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel5131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel5131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel5131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5131 = lv_label_create(ui_Panel5131);
    lv_obj_set_width(ui_Label5131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label5131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label5131, -91);
    lv_obj_set_y(ui_Label5131, 38);
    lv_obj_set_align(ui_Label5131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5131, "声音");
    lv_obj_set_style_text_font(ui_Label5131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider3131 = lv_slider_create(ui_Panel5131);
    lv_obj_set_width(ui_Slider3131, 140);
    lv_obj_set_height(ui_Slider3131, 5);
    lv_obj_set_x(ui_Slider3131, 27);
    lv_obj_set_y(ui_Slider3131, 38);
    lv_obj_set_align(ui_Slider3131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Slider3131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Slider3131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_slider_set_range(ui_Slider3131, 0, 30);

    ui_Panel1131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel1131, 35);
    lv_obj_set_width(ui_Panel1131, lv_pct(100));
    lv_obj_set_x(ui_Panel1131, -2);
    lv_obj_set_y(ui_Panel1131, 40);
    lv_obj_set_align(ui_Panel1131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel1131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel1131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel1131, LV_OBJ_FLAG_SCROLLABLE); /// Flags
    lv_obj_set_style_border_color(ui_Panel1131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel1131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel1131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6131 = lv_label_create(ui_Panel1131);
    lv_obj_set_width(ui_Label6131, LV_SIZE_CONTENT);  /// 1
    lv_obj_set_height(ui_Label6131, LV_SIZE_CONTENT); /// 1
    lv_obj_set_x(ui_Label6131, -91);
    lv_obj_set_y(ui_Label6131, 38);
    lv_obj_set_align(ui_Label6131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6131, "亮度");
    lv_obj_set_style_text_font(ui_Label6131, &lv_chinese_20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider2131 = lv_slider_create(ui_Panel1131);
    lv_obj_set_width(ui_Slider2131, 140);
    lv_obj_set_height(ui_Slider2131, 5);
    lv_obj_set_x(ui_Slider2131, 27);
    lv_obj_set_y(ui_Slider2131, 38);
    lv_obj_set_align(ui_Slider2131, LV_ALIGN_CENTER);
    lv_obj_add_flag(ui_Slider2131, LV_OBJ_FLAG_SCROLL_ON_FOCUS); /// Flags
    lv_obj_clear_flag(ui_Slider2131, LV_OBJ_FLAG_SCROLLABLE);    /// Flags
    lv_slider_set_range(ui_Slider2131, 1, 100);

    lv_slider_set_value(ui_Slider3131, stored_config.audio, LV_ANIM_OFF);
    lv_obj_add_event_cb(ui_Slider3131, slider_event_audio_cb, LV_EVENT_VALUE_CHANGED, NULL);

    lv_slider_set_value(ui_Slider2131, stored_config.bright, LV_ANIM_OFF);
    lv_obj_add_event_cb(ui_Slider2131, slider_event_bright_cb, LV_EVENT_VALUE_CHANGED, NULL);

    // 将时间设为最近
    now = Rtc.GetDateTime();
    char years_buf[100] = {0};
    int year = now.Year();
    snprintf(years_buf, 100, "%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d", year - 5, year - 4, year - 3, year - 2, year - 1, year, year + 1, year + 2, year + 3, year + 4, year + 5);
    lv_roller_set_options(ui_Roller2131, years_buf, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_Roller2131, 5, LV_ANIM_OFF);
    lv_roller_set_selected(ui_Roller7131, now.Month() - 1, LV_ANIM_OFF);
    lv_roller_set_selected(ui_Roller8131, now.Day() - 1, LV_ANIM_OFF);
    lv_roller_set_selected(ui_Roller1131, now.Hour(), LV_ANIM_OFF);
    lv_roller_set_selected(ui_Roller3131, now.Minute(), LV_ANIM_OFF);
    lv_roller_set_selected(ui_Roller9131, now.Second(), LV_ANIM_OFF);

    lv_obj_add_event_cb(ui_Roller2131, roller_set_time_year_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Roller7131, roller_set_time_month_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Roller8131, roller_set_time_day_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Roller1131, roller_set_time_hour_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Roller3131, roller_set_time_minute_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_Roller9131, roller_set_time_second_cb, LV_EVENT_VALUE_CHANGED, NULL);
}

void slider_event_bright_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    stored_config.bright = (int)lv_slider_get_value(slider);
    config_save_all();
}

void slider_event_audio_cb(lv_event_t *e)
{
    lv_obj_t *slider = lv_event_get_target(e);
    stored_config.audio = (int)lv_slider_get_value(slider);
    myDFPlayer.volume(stored_config.audio);
    config_save_all();
}

void roller_set_time_year_cb(lv_event_t *e)
{
    lv_obj_t *obj = lv_event_get_target(e);
    char years_buf[100] = {0};
    lv_roller_get_selected_str(obj, years_buf, 100);
    int year = atoi(years_buf);
    snprintf(years_buf, 100, "%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d\n%04d", year - 5, year - 4, year - 3, year - 2, year - 1, year, year + 1, year + 2, year + 3, year + 4, year + 5);
    lv_roller_set_options(ui_Roller2131, years_buf, LV_ROLLER_MODE_NORMAL);
    lv_roller_set_selected(ui_Roller2131, 5, LV_ANIM_OFF);

    RtcDateTime sync_time(year, now.Month(), now.Day(), now.Hour(), now.Minute(), now.Second());
    Rtc.SetDateTime(sync_time);
}

void roller_set_time_month_cb(lv_event_t *e)
{
    RtcDateTime sync_time(now.Year(), lv_roller_get_selected(ui_Roller7131) + 1, now.Day(), now.Hour(), now.Minute(), now.Second());
    Rtc.SetDateTime(sync_time);
}

void roller_set_time_day_cb(lv_event_t *e)
{
    RtcDateTime sync_time(now.Year(), now.Month(), lv_roller_get_selected(ui_Roller8131) + 1, now.Hour(), now.Minute(), now.Second());
    Rtc.SetDateTime(sync_time);
}

void roller_set_time_hour_cb(lv_event_t *e)
{
    RtcDateTime sync_time(now.Year(), now.Month(), now.Day(), lv_roller_get_selected(ui_Roller1131), now.Minute(), now.Second());
    Rtc.SetDateTime(sync_time);
}

void roller_set_time_minute_cb(lv_event_t *e)
{
    RtcDateTime sync_time(now.Year(), now.Month(), now.Day(), now.Hour(), lv_roller_get_selected(ui_Roller3131), now.Second());
    Rtc.SetDateTime(sync_time);
}

void roller_set_time_second_cb(lv_event_t *e)
{
    RtcDateTime sync_time(now.Year(), now.Month(), now.Day(), now.Hour(), now.Minute(), lv_roller_get_selected(ui_Roller9131));
    Rtc.SetDateTime(sync_time);
}
