#ifndef SHANG_DISPLAY_H
#define SHANG_DISPLAY_H

#include <lvgl.h>
#include <TFT_eSPI.h>
#include <Config.h>

extern TFT_eSPI tft;
extern lv_disp_draw_buf_t draw_buf;
extern lv_disp_drv_t disp_drv;              /*Descriptor of a display driver*/
extern lv_color_t buf1[screen_buffer_size]; /*Declare a buffer for 1/10 screen size*/
extern lv_color_t buf2[screen_buffer_size]; /*Declare a buffer for 1/10 screen size*/
extern lv_obj_t *screenMain;                // 创建一个实际的屏幕对象
extern lv_obj_t *labelTime;

LV_FONT_DECLARE(lv_font_number_48);
void initDisplay(void);
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p);

#endif