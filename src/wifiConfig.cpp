#include <wifiConfig.h>

void WiFiConnect()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin( //connect to wifi
      ssid, //ssid
      password //password
    ); 
    while (WiFi.waitForConnectResult() != WL_CONNECTED)
    {   //wait for connection
        Serial.println("Connection Failed! Rebooting"); //println meaning print everything within a single line
        delay(5000); //wait 5 seconds
        ESP.restart(); //restart the ESP
    }
    Serial.println("");
    Serial.println("WiFi connected"); 
    Serial.println("IP address: "); //print IP address
    Serial.println(WiFi.localIP()); //print IP address
    Serial.printf("RSSI: %d\n", WiFi.RSSI()); //print RSSI
}