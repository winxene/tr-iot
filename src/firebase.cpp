#include "firebase.h"
#include "dispenser.h"
#define FIREBASE_HOST "https://drinkify-firebase-default-rtdb.asia-southeast1.firebasedatabase.app/"
#define FIREBASE_AUTH "5V6BSGqtHdj6Jj2o6FVqQibT9i0UNWwaO5HrAktI"
FirebaseData fbdo;
FirebaseConfig fbConfig;
FirebaseData fbdoStream;

void Firebase_Init(const String &streamPath)
{
  FirebaseAuth fbAuth;
  fbConfig.host = FIREBASE_HOST;
  fbConfig.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&fbConfig, &fbAuth);
  Firebase.reconnectWiFi(true);

#if defined(ESP8266)
  fbdo.setBSSLBufferSize(2 * 1024, 1024);
#endif

  fbdo.setResponseSize(1024);
  Firebase.RTDB.setwriteSizeLimit(&fbdo, "small");
  while (!Firebase.ready())
  {
    Serial.println("Connecting to firebase...");
    delay(1000);
  }
  String path = streamPath;
  if (Firebase.RTDB.beginStream(&fbdoStream, path.c_str()))
  {
    Serial.println("Firebase stream on " + path);
    Firebase.RTDB.setStreamCallback(&fbdoStream, onFirebaseStream, onFirebaseStreamTimeout);
  }
  else
    Serial.println("Firebase stream failed: " + fbdoStream.errorReason());
}

void onFirebaseStreamTimeout(bool timeout) 
{
  // Serial.printf("Firebase stream timeout: %d\n", timeout);
}

void onFirebaseStream(FirebaseStream dataGathered)
{
    Serial.printf("onFirebaseStream: %s %s %s %s\n",
    dataGathered.streamPath().c_str(),
    dataGathered.dataPath().c_str(), dataGathered.dataType().c_str(),
    dataGathered.stringData().c_str());
    // if (dataGathered.dataType() == "string")
    // {
    //   String data = dataGathered.stringData();
    //   if (data == "true")
    //   {
    //     Serial.println("Pump is ON");
    //     pumpLogic(true);
    //   }
    //   else if (data == "false")
    //   {
    //     Serial.println("Pump is OFF");
    //     pumpLogic(false);
    //   }
    // }
    // if(dataGathered.dataType() == "bool")
    // { 
      if(dataGathered.boolData() == true)
      {
          Serial.println("Pump is ON");
          pumpLogic(true);
      }
      else
      {
          Serial.println("Pump is OFF");
          pumpLogic(false);
      }
    // }
}

void pushToFirebase(const char *id,const char *token)
{
    char updatedToken[255];
    char updatedStatus[255];
    strcpy(updatedToken, "/");
    strcat(updatedToken, id);
    strcat(updatedToken, "/dispenserData/token");

    strcpy(updatedStatus, "/");
    strcat(updatedStatus, id);
    strcat(updatedStatus, "/phoneData/status");

  Firebase.RTDB.setString(&fbdo, updatedToken, token);
  Firebase.RTDB.setBool(&fbdo, updatedStatus, false);
  // Firebase.RTDB.setBool(&fbdo, updatedStatus, false);
  // Serial.println("Dispenser Logic: OFF");
}