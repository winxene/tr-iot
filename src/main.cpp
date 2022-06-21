#include <myDisplay.h>
#include <firebase.h>
#include <wifiConfig.h>
#include <pump.h>

void setup()
{
  Serial.begin(115200);
  WiFiConnect();
  
  //from pump
  pumpSetup();

  display_init();
  display_driver_init();
  ui_init();
  
  Firebase_Init("/D-1/phoneData");
  pushToFirebase("/D-1", " ");
}

void loop() {
  lv_timer_handler();
  delay(5);
}

