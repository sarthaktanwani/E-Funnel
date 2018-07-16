/*
 * This code will read data on thingspeak.
 *           --"MADE BY AYUSH AGGARWAL"
 */
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>
char ssid[] = "Shanu";    //  your network SSID (name) 
char pass[] = "20167635";   // your network password
//int status = WL_IDLE_STATUS;
WiFiClient  client;

unsigned long Channel= 511722;
unsigned int times = 1;

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  ThingSpeak.begin(client);  
}

void loop() 
{
  float nt = ThingSpeak.readFloatField(Channel, times);
  if(nt!=0)
  {
    Serial.print("Petrol "); 
    Serial.print(nt);
    Serial.println(" L"); 
  }
  delay(30000); // Note that the weather station only updates once a minute
}
