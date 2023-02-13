#include "display.h"

static TFT_eSPI tft = TFT_eSPI(screen_width, screen_height); /* TFT instance */
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;                                       /*Descriptor of a display driver*/
static void *buf1 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/
static void *buf2 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/

static char time_string[time_string_size] = "加载中";
static char humidity_string[humidity_string_size] = "";
static char temperature_string[temperature_string_size] = "";
static char date_string[date_string_size] = "";
static const String WDAY_NAMES[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}; // 星期
float humidity = 0.0;
float temperature = 0.0;

void init_page_loading(lv_obj_t *Page);
void init_page_main(lv_obj_t *Page);
void timer_page_main(lv_timer_t *timer);
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

void init_page_main(lv_obj_t *Page)
{
    lv_obj_t *labelTime = lv_label_create(Page);
    lv_obj_t *labelHumidity = lv_label_create(Page);
    lv_obj_t *labelTemperature = lv_label_create(Page);
    lv_obj_t *labelDate = lv_label_create(Page);
    static lv_obj_t *children[4] = {labelTime, labelHumidity, labelTemperature, labelDate};

    lv_obj_center(labelTime);
    lv_obj_set_style_text_font(labelTime, &lv_chinese_48, 0);
    lv_label_set_text(labelTime, time_string);
    lv_obj_set_align(labelHumidity, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_font(labelHumidity, &lv_chinese_20, 0);
    lv_label_set_text(labelHumidity, humidity_string);
    lv_obj_set_align(labelTemperature, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(labelTemperature, &lv_chinese_20, 0);
    lv_label_set_text(labelTemperature, temperature_string);
    lv_obj_set_align(labelDate, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_text_font(labelDate, &lv_chinese_20, 0);
    lv_label_set_text(labelDate, date_string);

    lv_timer_t *timer = lv_timer_create(timer_page_main, 200, children);
}

void init_page_counter(lv_obj_t *Page){

}


void timer_page_main(lv_timer_t *timer)
{
    static struct tm timeinfo;
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

void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushColors(&color_p->full, w * h, true);
    tft.endWrite();

    lv_disp_flush_ready(disp);
}

void display_init(void)
{
    tft.init();
    tft.setRotation(0);
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_buffer_size); /*Initialize the display buffer.*/
    // lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screen_buffer_size); /*Initialize the display buffer.*/
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;    /*Assign the buffer to the display*/
    disp_drv.hor_res = screen_width;  /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = screen_height; /*Set the verizontal resolution of the display*/
    lv_disp_drv_register(&disp_drv);  /*Finally register the driver*/
    input_init();


    lv_obj_t *screen = lv_obj_create(NULL);
    lv_obj_t *tabview = lv_tabview_create(screen, LV_DIR_BOTTOM, 30);
    lv_obj_set_style_text_font(tabview, &lv_chinese_20, 0);
    lv_obj_t *tabtime = lv_tabview_add_tab(tabview, "时间");
    lv_obj_t *tabclock = lv_tabview_add_tab(tabview, "闹钟");
    lv_obj_t *tabcounter = lv_tabview_add_tab(tabview, "计时");

    init_page_main(tabtime);
    lv_scr_load(screen);
    display_routine();
}

void display_routine()
{
    lv_task_handler();
    vTaskDelay(1);
}
