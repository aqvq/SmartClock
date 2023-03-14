#include "display.h"

static TFT_eSPI tft = TFT_eSPI(screen_width, screen_height); /* TFT instance */
static lv_disp_draw_buf_t draw_buf;
static lv_disp_drv_t disp_drv;                                       /*Descriptor of a display driver*/
static void *buf1 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/
#if USE_DOUBLE_BUFFER
static void *buf2 = malloc(sizeof(lv_color_t) * screen_buffer_size); /*Declare a buffer for 1/10 screen size*/
#endif
lv_obj_t *screen;

extern int time_status;
extern uint8_t status;
extern unsigned long initialized_time;
extern StoredConfig stored_config;
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
    ledcSetup(0, 5000, 12);
    ledcAttachPin(screen_light_pin, 0);

    tft.init();
    tft.fillScreen(0);
    // tft.invertDisplay(true);
    tft.setRotation(0);
    lv_init();
#if USE_DOUBLE_BUFFER
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_buffer_size); /*Initialize the display buffer.*/
#else
    lv_disp_draw_buf_init(&draw_buf, buf1, NULL, screen_buffer_size); /*Initialize the display buffer.*/
#endif
    lv_disp_drv_init(&disp_drv); /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;
    disp_drv.draw_buf = &draw_buf;    /*Assign the buffer to the display*/
    disp_drv.hor_res = screen_width;  /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = screen_height; /*Set the verizontal resolution of the display*/
    lv_disp_drv_register(&disp_drv);  /*Finally register the driver*/
    input_init();

    screen = lv_obj_create(NULL);
    // 采用TabView进行页面切换
    lv_obj_t *tabview = lv_tabview_create(screen, LV_DIR_BOTTOM, 30);
    lv_obj_set_style_text_font(tabview, &lv_chinese_20, 0);

    lv_obj_t *tab_time = lv_tabview_add_tab(tabview, "时间");
    lv_obj_t *tab_count = lv_tabview_add_tab(tabview, "计时");
    lv_obj_t *tab_settings = lv_tabview_add_tab(tabview, "设置");
    lv_obj_t *tab_clock = lv_tabview_add_tab(tabview, "闹钟");

    // 初始化控件
    ui_time_init(tab_time);
    ui_count_init(tab_count);
    ui_settings_init(tab_settings);
    ui_clock_list_init(tab_clock);
    ui_clock_create_init(tab_clock);
    // 加载屏幕
    lv_scr_load(screen);
}

void display_routine()
{
#if ENABLE_SLEEP
    if (time_status == NORMAL && status == NOTSTART && lv_disp_get_inactive_time(NULL) > sleep_time && millis() - initialized_time > sleep_time)
        ledcWrite(0, int(brightness * 4096));
    else
        ledcWrite(0, 4096);
#else
    ledcWrite(0, (int)(stored_config.bright / 100.0 * 4096));
#endif

    // vTaskDelay(1);
    lv_task_handler();
}
