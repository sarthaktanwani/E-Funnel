/*
 * This code will check if you have an internet connection or not.If positive(that is you are connected to internet) then data will be uploaded to "THINGSPEAK".    
 * MADE BY AYUSH AGGARWAL.
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "ThingSpeak.h"

unsigned long Channel= 511722;
unsigned int t=2;
int addr = 2;
int i=0;
const char *ssid = "Shanu";
const char *password = "20167635";
const char *host = "api.thingspeak.com";
String apiKey = "XB7NAHQUZTG7QKS3";

WiFiClient  client;

void LCD1()
{
  lcd.setCursor(0,0);
  lcd.print("        ");
  lcd.setCursor(0,1);
  lcd.print("        ");
  delay(1000);
  lcd.setCursor(0,0);
}

void wifi()
{
  LCD1();
  lcd.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    lcd.print(".");
    t++;
    if(t>10)
    {
      lcd.print("Wifi Not Found");
      delay(1000);
    }
  }
  lcd.print("Connected to:");
  lcd.println(ssid);
  delay(2000);
  
  WiFiClient client;          
  const int httpPort = 80; //Port 80 is commonly used for www
  //Connect to host, host(web site) is define at top 
  lcd.print("Connecting");
  delay(1000);
  if(!client.connect(host, httpPort))
  {
    lcd.println("No Internet");
    delay(1000);
  }
  else                                                                                                                               // If wifi is connected
  {
      String ADCData;
      float adcvalue=98.77;  //Read Analog value of LDR   
      ADCData = String(adcvalue);   //String to interger conversion 
      String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
      Link = Link + ADCData;
      Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
      client.print(Link);
      delay(100);
      lcd.print("Upload Complete");
      delay(1000);
  }  
}

void setup() 
{
  lcd.begin(16, 2); 
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  ThingSpeak.begin(client);
}

void loop() 
{
  wifi();
  delay(5000000);
}
