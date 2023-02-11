#include <Arduino.h>
#include <TFT_eSPI.h>
#include <lvgl.h>
#include <WiFi.h>
#include <Config.h>
#include <Drivers/Time.h>
#include <Drivers/Network.h>
#include <Drivers/Display.h>

TFT_eSPI tft = TFT_eSPI(screen_width, screen_height); /* TFT instance */
lv_disp_draw_buf_t draw_buf;
lv_disp_drv_t disp_drv;              /*Descriptor of a display driver*/
lv_color_t buf1[screen_buffer_size]; /*Declare a buffer for 1/10 screen size*/
lv_color_t buf2[screen_buffer_size]; /*Declare a buffer for 1/10 screen size*/
char time_string[time_string_size] = "00:00:00";
struct tm timeinfo;
lv_obj_t *screenMain; // 创建一个实际的屏幕对象
lv_obj_t *labelTime;

// LV_FONT_DECLARE(lv_font_number_60);
LV_FONT_DECLARE(lv_font_number_48);

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  initDisplay();
  initWiFi();
  configTime(8 * 3600, 0, ntp_server);
}

void loop()
{
  // put your main code here, to run repeatedly:
  updateTime(&timeinfo);
  sprintf(time_string, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  lv_label_set_text(labelTime, time_string);

  lv_scr_load(screenMain);
  lv_task_handler();
  vTaskDelay(1);
  delay(500);
}
