#include <NewPing.h>

#define TRIGGER_PIN 14  // Arduino pin tied to trigger pin on the ultrasonic sensor.
// #define ECHO_PIN    13// Arduino pin tied to echo pin on the ultrasonic sensor.
#define ECHO_PIN    12// Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
// const int  dpm = 21;
const int  dpm = 22;
// const int  dpm = 2;
extern NewPing sonar;

void pumpLogic(bool pump);

