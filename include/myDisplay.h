#define LGFX_AUTODETECT // Autodetect board
#define LGFX_USE_V1
#include <LovyanGFX.hpp>//display driver, SPI etc.
#include <lvgl.h>
#include <lv_conf.h>

/*** Setup screen resolution for LVGL ***/
static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 480;
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[screenWidth * 10];

static LGFX myDisplay;

LV_IMG_DECLARE(drinkify);


// lv_obj_t * lv_qrcode_create(lv_obj_t * parent, lv_coord_t size, lv_color_t dark_color, lv_color_t light_color);
// lv_res_t lv_qrcode_update(lv_obj_t * qrcode, const void * data, uint32_t data_len);
// void ui_start_button();
// void ui_dynamic_obj(void);
//insert in void setup in main.cpp
void display_init();
void display_driver_init();
void ui_init();
