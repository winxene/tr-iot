#ifndef __FIREBASE_H__
#define __FIREBASE_H__
#include <Firebase_ESP_Client.h>
#include <lvgl.h>
extern FirebaseData fbdo;
extern bool showQRCode;
extern lv_obj_t * qrCode;
void Firebase_Init(const String& streamPath);
void onFirebaseStream(FirebaseStream data);
void onFirebaseStreamTimeout(bool timeout);
void pushToFirebase(const char *id,const char *token);
#endif