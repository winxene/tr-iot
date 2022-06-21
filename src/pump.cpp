#include <pump.h>
#include <firebase.h>

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.// const int dpm = 23; //diaphragm pump motor , temp dikomen dlu   
int timeCounter = 0;
int8_t targetTime = 268;

void pumpSetup()
{
  pinMode(dpm, OUTPUT);
}

void pumpLogic(bool pump)
{
  //HIGH= off, LOW= on due to active low
  while(pump){

    delay(50);
    int cm = sonar.ping_cm();
    if (cm > 1 && cm < 6){
      timeCounter++;
      Serial.printf("on timer: %d\n",timeCounter);
      digitalWrite(dpm, LOW);
      if(timeCounter>=targetTime){
        digitalWrite(dpm, HIGH);
        Serial.println("done~!");
        pushToFirebase("/D-1", " ");
        timeCounter=0;
        pump = false;
        break;
      }
      delay(10);
    }
    else digitalWrite(dpm, HIGH);
  }
}
