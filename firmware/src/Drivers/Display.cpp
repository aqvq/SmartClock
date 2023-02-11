#include <Drivers/Display.h>

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

void initDisplay(void)
{
    tft.init();
    tft.setRotation(0);
    lv_init();
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, screen_buffer_size); /*Initialize the display buffer.*/
    lv_disp_drv_init(&disp_drv);                                      /*Basic initialization*/
    disp_drv.flush_cb = my_disp_flush;                                /*Set your driver function*/
    disp_drv.draw_buf = &draw_buf;                                    /*Assign the buffer to the display*/
    disp_drv.hor_res = screen_width;                                  /*Set the horizontal resolution of the display*/
    disp_drv.ver_res = screen_height;                                 /*Set the verizontal resolution of the display*/
    lv_disp_drv_register(&disp_drv);                                  /*Finally register the driver*/

    screenMain = lv_obj_create(NULL); // 创建一个实际的屏幕对象
    labelTime = lv_label_create(screenMain);

    lv_obj_set_size(screenMain, screen_width, screen_height); // Or in one function
    lv_obj_center(labelTime);
    lv_obj_set_style_text_font(labelTime, &lv_font_number_48, 0);
    lv_label_set_text(labelTime, "Loading");
    lv_scr_load(screenMain);
    lv_task_handler();
    vTaskDelay(1);
}