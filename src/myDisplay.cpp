#include <lv_conf.h>
#include <myDisplay.h>
#include <firebase.h>

lv_obj_t * mainScreen;
lv_obj_t * qrCode; 
lv_obj_t *logoImage;
lv_obj_t * startButton;
char text [255];


void displayFlush( lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p )
{
  uint32_t w = ( area->x2 - area->x1 + 1 );
  uint32_t h = ( area->y2 - area->y1 + 1 );

  myDisplay.startWrite();
  myDisplay.setAddrWindow( area->x1, area->y1, w, h );
  myDisplay.writePixels((lgfx::rgb565_t *)&color_p->full, w * h);
  myDisplay.endWrite();
  lv_disp_flush_ready(disp);
}

void readTouchPad( lv_indev_drv_t * indev_driver, lv_indev_data_t * data )
{
    uint16_t touchX, touchY;
    bool touched = myDisplay.getTouch(&touchX, &touchY);
    if (!touched)
    {
      data->state = LV_INDEV_STATE_REL;
    }
    else
    {
      data->state = LV_INDEV_STATE_PR;
      /*Set the coordinates*/
      data->point.x = touchX;
      data->point.y = touchY;
    }
}

void display_init() {
  myDisplay.begin();
  myDisplay.init();
  myDisplay.setBrightness(255);
  lv_init();
  lv_disp_draw_buf_init( &draw_buf, buf, NULL, screenWidth * 10 );
}

void display_driver_init() {
  static lv_disp_drv_t disp_drv;
  lv_disp_drv_init(&disp_drv);

  disp_drv.hor_res = screenWidth;
  disp_drv.ver_res = screenHeight;
  disp_drv.flush_cb = displayFlush;
  disp_drv.draw_buf = &draw_buf;
  lv_disp_drv_register(&disp_drv);

  static lv_indev_drv_t indev_drv;
  lv_indev_drv_init(&indev_drv);
  indev_drv.type = LV_INDEV_TYPE_POINTER;
  indev_drv.read_cb = readTouchPad;
  lv_indev_drv_register(&indev_drv);
}

//taken from https://codereview.stackexchange.com/questions/29198/random-string-generator-in-c
void generateString(size_t size){
  char stringResult[255];
  static const char alphaNumeric[]=         
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
  if(size){
    --size;
    size_t increment =0;
    while(increment<size){
      int key = rand() % (int) (sizeof alphaNumeric - 1);
      stringResult[increment] = alphaNumeric[key];
      increment++;
    }
    stringResult[size] = '\0'; // \0 indicates end string.
  }
  Serial.println(stringResult);
  strcpy(text, stringResult);
}

void generateQRCode(lv_event_t * e){
  lv_event_code_t code = lv_event_get_code(e);
  if (code == LV_EVENT_CLICKED || code == LV_EVENT_FOCUSED) {
    generateString(17);
    lv_qrcode_update(qrCode, text, strlen(text));
    pushToFirebase("/D-1", text);
    lv_obj_clear_flag(qrCode, LV_OBJ_FLAG_HIDDEN);
    bool showQRCode=true;
    int clock= 157;
    int counter=0;
    while(showQRCode){
      counter++;
      delay(50);
      if(counter>=clock)
      {
        lv_obj_add_flag(qrCode, LV_OBJ_FLAG_HIDDEN);
        counter=0;
        showQRCode=false;
        break;
      }
      delay(10);
    }
  }
}

void titleLabel(){  
  lv_obj_t * titleLabel = lv_label_create(mainScreen);
  lv_obj_set_pos(titleLabel, 0, 150);
  lv_obj_set_align(titleLabel, LV_ALIGN_TOP_MID);
  lv_label_set_text(titleLabel, "Tap Start to Generate QR-Code");
  lv_obj_clear_flag(titleLabel, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_set_style_text_font(titleLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void ui_background(){
  mainScreen = lv_obj_create(NULL);
  lv_obj_clear_flag(mainScreen, LV_OBJ_FLAG_SCROLLABLE); //cannot scroll
  logoImage = lv_img_create(mainScreen); 
  lv_img_set_src(logoImage, &drinkify);
  lv_obj_set_size(logoImage, 177, 68);
  lv_obj_set_pos(logoImage, 0, 20);
  lv_obj_set_align(logoImage, LV_ALIGN_TOP_MID);
  lv_obj_add_flag(logoImage, LV_OBJ_FLAG_ADV_HITTEST);
  lv_obj_clear_flag(logoImage, LV_OBJ_FLAG_SCROLLABLE);
  titleLabel();
}

void ui_start_button(){
  lv_obj_t * label;
  startButton = lv_btn_create(mainScreen);
  lv_obj_add_event_cb(startButton, generateQRCode, LV_EVENT_ALL, NULL);
  
  lv_obj_set_size(startButton, 120, 50);  
  lv_obj_align(startButton, LV_ALIGN_BOTTOM_MID, 0, -10);
  lv_obj_clear_flag(startButton, LV_OBJ_FLAG_HIDDEN);
 
  label = lv_label_create(startButton);
  lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);
  lv_label_set_text(label, "Start!");
}

void ui_dynamic_obj(void) {
  ui_start_button();
  lv_obj_add_flag(qrCode, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_pos(qrCode, 0, -150);
  lv_obj_set_align(qrCode, LV_ALIGN_BOTTOM_MID);
}

void ui_init() {
  ui_background();
  qrCode = lv_qrcode_create(mainScreen, 140, lv_color_hex3(0x000), lv_color_hex3(0xeef));
  ui_dynamic_obj();
  lv_disp_load_scr(mainScreen);
}

