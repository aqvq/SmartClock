// SquareLine LVGL GENERATED FILE
// EDITOR VERSION: SquareLine Studio 1.2.1
// LVGL VERSION: 8.3.4
// PROJECT: SquareLine_Project

#include "ui.h"
#include "ui_helpers.h"

///////////////////// VARIABLES ////////////////////
lv_obj_t * ui_Screen1;
lv_obj_t * ui_Panel3131;
lv_obj_t * ui_Panel2131;
lv_obj_t * ui_Roller2131;
lv_obj_t * ui_Label1131;
lv_obj_t * ui_Roller7131;
lv_obj_t * ui_Label2131;
lv_obj_t * ui_Roller8131;
lv_obj_t * ui_Label3131;
lv_obj_t * ui_Panel6131;
lv_obj_t * ui_Roller1131;
lv_obj_t * ui_Label4131;
lv_obj_t * ui_Roller3131;
lv_obj_t * ui_Label7131;
lv_obj_t * ui_Roller9131;
lv_obj_t * ui_Label8131;
lv_obj_t * ui_Panel5131;
lv_obj_t * ui_Label5131;
lv_obj_t * ui_Slider3131;
lv_obj_t * ui_Panel1131;
lv_obj_t * ui_Label6131;
lv_obj_t * ui_Slider2131;

///////////////////// TEST LVGL SETTINGS ////////////////////
#if LV_COLOR_DEPTH != 16
    #error "LV_COLOR_DEPTH should be 16bit to match SquareLine Studio's settings"
#endif
#if LV_COLOR_16_SWAP !=1
    #error "LV_COLOR_16_SWAP should be 1 to match SquareLine Studio's settings"
#endif

///////////////////// ANIMATIONS ////////////////////

///////////////////// FUNCTIONS ////////////////////

///////////////////// SCREENS ////////////////////
void ui_Screen1_screen_init(void)
{
    ui_Screen1 = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_bg_color(ui_Screen1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Screen1, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel3131 = lv_obj_create(ui_Screen1);
    lv_obj_set_width(ui_Panel3131, lv_pct(100));
    lv_obj_set_height(ui_Panel3131, lv_pct(100));
    lv_obj_set_x(ui_Panel3131, 0);
    lv_obj_set_y(ui_Panel3131, -2);
    lv_obj_set_align(ui_Panel3131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel3131, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_Panel3131, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel3131, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel3131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel3131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_Panel3131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel2131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel2131, 40);
    lv_obj_set_width(ui_Panel2131, lv_pct(100));
    lv_obj_set_x(ui_Panel2131, 3);
    lv_obj_set_y(ui_Panel2131, -78);
    lv_obj_set_align(ui_Panel2131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel2131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel2131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel2131, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel2131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel2131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel2131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel2131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller2131 = lv_roller_create(ui_Panel2131);
    lv_roller_set_options(ui_Roller2131, "2021\n2022\n2023", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller2131, 55);
    lv_obj_set_height(ui_Roller2131, 30);
    lv_obj_set_align(ui_Roller2131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller2131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller2131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller2131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller2131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label1131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label1131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label1131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label1131, -17);
    lv_obj_set_y(ui_Label1131, -8);
    lv_obj_set_align(ui_Label1131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label1131, "年");
    lv_obj_set_style_text_font(ui_Label1131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller7131 = lv_roller_create(ui_Panel2131);
    lv_roller_set_options(ui_Roller7131, "10\n11\n12", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller7131, 35);
    lv_obj_set_height(ui_Roller7131, 30);
    lv_obj_set_align(ui_Roller7131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller7131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller7131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller7131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller7131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label2131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label2131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label2131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label2131, 2);
    lv_obj_set_y(ui_Label2131, -56);
    lv_obj_set_align(ui_Label2131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label2131, "月");
    lv_obj_set_style_text_font(ui_Label2131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller8131 = lv_roller_create(ui_Panel2131);
    lv_roller_set_options(ui_Roller8131, "25\n26\n27\n28", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller8131, 35);
    lv_obj_set_height(ui_Roller8131, 30);
    lv_obj_set_align(ui_Roller8131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller8131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller8131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller8131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller8131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label3131 = lv_label_create(ui_Panel2131);
    lv_obj_set_width(ui_Label3131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label3131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label3131, 63);
    lv_obj_set_y(ui_Label3131, -53);
    lv_obj_set_align(ui_Label3131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label3131, "日");
    lv_obj_set_style_text_font(ui_Label3131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel6131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel6131, 40);
    lv_obj_set_width(ui_Panel6131, lv_pct(100));
    lv_obj_set_x(ui_Panel6131, 3);
    lv_obj_set_y(ui_Panel6131, -78);
    lv_obj_set_align(ui_Panel6131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel6131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel6131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel6131, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel6131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel6131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel6131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel6131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller1131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller1131, "20\n21\n22", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller1131, 35);
    lv_obj_set_height(ui_Roller1131, 30);
    lv_obj_set_align(ui_Roller1131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller1131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller1131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller1131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller1131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label4131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label4131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label4131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label4131, -17);
    lv_obj_set_y(ui_Label4131, -8);
    lv_obj_set_align(ui_Label4131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label4131, "时");
    lv_obj_set_style_text_font(ui_Label4131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller3131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller3131, "10\n11\n12", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller3131, 35);
    lv_obj_set_height(ui_Roller3131, 30);
    lv_obj_set_align(ui_Roller3131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller3131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller3131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller3131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller3131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label7131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label7131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label7131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label7131, 2);
    lv_obj_set_y(ui_Label7131, -56);
    lv_obj_set_align(ui_Label7131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label7131, "分");
    lv_obj_set_style_text_font(ui_Label7131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Roller9131 = lv_roller_create(ui_Panel6131);
    lv_roller_set_options(ui_Roller9131, "25\n26\n27\n28", LV_ROLLER_MODE_NORMAL);
    lv_obj_set_width(ui_Roller9131, 35);
    lv_obj_set_height(ui_Roller9131, 30);
    lv_obj_set_align(ui_Roller9131, LV_ALIGN_CENTER);

    lv_obj_set_style_text_color(ui_Roller9131, lv_color_hex(0x000000), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_Roller9131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_Roller9131, lv_color_hex(0xFFFFFF), LV_PART_SELECTED | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_Roller9131, 255, LV_PART_SELECTED | LV_STATE_DEFAULT);

    ui_Label8131 = lv_label_create(ui_Panel6131);
    lv_obj_set_width(ui_Label8131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label8131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label8131, 63);
    lv_obj_set_y(ui_Label8131, -53);
    lv_obj_set_align(ui_Label8131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label8131, "秒");
    lv_obj_set_style_text_font(ui_Label8131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Panel5131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel5131, 35);
    lv_obj_set_width(ui_Panel5131, lv_pct(100));
    lv_obj_set_x(ui_Panel5131, 0);
    lv_obj_set_y(ui_Panel5131, 81);
    lv_obj_set_align(ui_Panel5131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel5131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel5131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel5131, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel5131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel5131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel5131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel5131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label5131 = lv_label_create(ui_Panel5131);
    lv_obj_set_width(ui_Label5131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label5131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label5131, -91);
    lv_obj_set_y(ui_Label5131, 38);
    lv_obj_set_align(ui_Label5131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label5131, "声音");
    lv_obj_set_style_text_font(ui_Label5131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider3131 = lv_slider_create(ui_Panel5131);
    lv_obj_set_width(ui_Slider3131, 120);
    lv_obj_set_height(ui_Slider3131, 5);
    lv_obj_set_x(ui_Slider3131, 27);
    lv_obj_set_y(ui_Slider3131, 38);
    lv_obj_set_align(ui_Slider3131, LV_ALIGN_CENTER);

    ui_Panel1131 = lv_obj_create(ui_Panel3131);
    lv_obj_set_height(ui_Panel1131, 35);
    lv_obj_set_width(ui_Panel1131, lv_pct(100));
    lv_obj_set_x(ui_Panel1131, -2);
    lv_obj_set_y(ui_Panel1131, 40);
    lv_obj_set_align(ui_Panel1131, LV_ALIGN_CENTER);
    lv_obj_set_flex_flow(ui_Panel1131, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(ui_Panel1131, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(ui_Panel1131, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_border_color(ui_Panel1131, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_Panel1131, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_Panel1131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_Panel1131, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Label6131 = lv_label_create(ui_Panel1131);
    lv_obj_set_width(ui_Label6131, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_Label6131, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_Label6131, -91);
    lv_obj_set_y(ui_Label6131, 38);
    lv_obj_set_align(ui_Label6131, LV_ALIGN_CENTER);
    lv_label_set_text(ui_Label6131, "亮度");
    lv_obj_set_style_text_font(ui_Label6131, &ui_font_chinese, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_Slider2131 = lv_slider_create(ui_Panel1131);
    lv_obj_set_width(ui_Slider2131, 120);
    lv_obj_set_height(ui_Slider2131, 5);
    lv_obj_set_x(ui_Slider2131, 27);
    lv_obj_set_y(ui_Slider2131, 38);
    lv_obj_set_align(ui_Slider2131, LV_ALIGN_CENTER);

}

void ui_init(void)
{
    lv_disp_t * dispp = lv_disp_get_default();
    lv_theme_t * theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED),
                                               false, LV_FONT_DEFAULT);
    lv_disp_set_theme(dispp, theme);
    ui_Screen1_screen_init();
    lv_disp_load_scr(ui_Screen1);
}
