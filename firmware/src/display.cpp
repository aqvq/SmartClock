#include "display.h"

static TFT_eSPI tft = TFT_eSPI(screen_width, screen_height); /* TFT instance */
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;                                       /*Descriptor of a display driver*/
static void *buf1 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/
// static void *buf2 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/

static char time_string[time_string_size] = "LOADING";
static char humidity_string[humidity_string_size] = "LOADING";
static char temperature_string[temperature_string_size] = "LOADING";
static char date_string[date_string_size] = "LOADING";
static lv_obj_t *screenMain; // 创建一个实际的屏幕对象
static lv_obj_t *labelTime;
static lv_obj_t *labelHumidity;
static lv_obj_t *labelTemperature;
static lv_obj_t *labelDate;

static struct tm timeinfo;
float humidity = 0.0;
float temperature = 0.0;

LV_FONT_DECLARE(lv_font_number_48);
LV_FONT_DECLARE(lv_font_number_20);

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
    // lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_buffer_size); /*Initialize the display buffer.*/
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screen_buffer_size); /*Initialize the display buffer.*/
    lv_disp_drv_init(&disp_drv);                                      /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;    /*Assign the buffer to the display*/
    disp_drv.hor_res = screen_width;  /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = screen_height; /*Set the verizontal resolution of the display*/
    lv_disp_drv_register(&disp_drv);  /*Finally register the driver*/

    screenMain = lv_obj_create(NULL); // 创建一个实际的屏幕对象
    labelTime = lv_label_create(screenMain);
    labelHumidity = lv_label_create(screenMain);
    labelTemperature = lv_label_create(screenMain);
    labelDate = lv_label_create(screenMain);

    lv_obj_set_size(screenMain, screen_width, screen_height); // Or in one function

    lv_obj_center(labelTime);
    lv_obj_set_style_text_font(labelTime, &lv_font_number_48, 0);
    lv_label_set_text(labelTime, time_string);

    lv_obj_set_align(labelTemperature, LV_ALIGN_TOP_LEFT);
    lv_obj_set_style_text_font(labelTemperature, &lv_font_number_20, 0);
    lv_label_set_text(labelTemperature, temperature_string);

    lv_obj_set_align(labelHumidity, LV_ALIGN_TOP_RIGHT);
    lv_obj_set_style_text_font(labelHumidity, &lv_font_number_20, 0);
    lv_label_set_text(labelHumidity, humidity_string);

    lv_obj_set_align(labelDate, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_style_text_font(labelDate, &lv_font_number_20, 0);
    lv_label_set_text(labelDate, date_string);

    lv_scr_load(screenMain);
    lv_task_handler();
    vTaskDelay(1);
}

void display_routine()
{
    getLocalTime(&timeinfo);
    if (WiFi.status() == WL_CONNECTED)
    {
        const String WDAY_NAMES[] = {"星期日", "星期一", "星期二", "星期三", "星期四", "星期五", "星期六"}; // 星期

        sprintf(time_string, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
        sprintf(date_string, "%04d年%02d月%02d日 %s", timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, WDAY_NAMES[timeinfo.tm_wday]);
        sprintf(humidity_string, "湿度: %2.2f%%", humidity);
        sprintf(temperature_string, "温度: %2.2f°C", temperature);
    }
    else
    {
        sprintf(time_string, "__:__:__");
        sprintf(date_string, "____年__月__日 星期__");
        sprintf(humidity_string, "湿度: NaN");
        sprintf(temperature_string, "温度: NaN");
    }
    lv_label_set_text(labelTime, time_string);
    lv_label_set_text(labelTemperature, temperature_string);
    lv_label_set_text(labelHumidity, humidity_string);
    lv_label_set_text(labelDate, date_string);

    //===================================

    dht_update(&humidity, &temperature);

    lv_scr_load(screenMain);
    lv_task_handler();
    vTaskDelay(1);
}
