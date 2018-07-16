/*  This code will check if you have an internet connection or not.
 *                     ->If positive(that is you are connected to internet) then data will be uploaded to "THINGSPEAK".
 *                     ->If negative(that is you are not connected to internet) then data will be uploaded to EEPROM(data will not be lost) and will be uploaded to "THINGSPEAK" next time.
 *                     
 *   Connections:-         
 *                          Modules                                               Nodemcu
 *       1)      Liquid Crystal Display                          
 *                                        RS                 =                    D0
 *                                        EN                 =                    D1
 *                                        D4                 =                    D2
 *                                        D5                 =                    D3
 *                                        D6                 =                    D4
 *                                        D7                 =                    D5
 *                                        
 *       2)      Hall Effect Sensor YF-S 201
 *                                        Data Pin           =                    D7
 *                
 *               -- "MADE BY AYUSH AGGARWAL"
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);

#include <EEPROM.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include "ThingSpeak.h"

float calibrationFactor = 4.5;
volatile byte pulseCount; 
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;

int val1=0;
int val2=0;
float val=00.00;
unsigned long Channel= 511722;
unsigned int t=2;
int addr = 2;
int i=0;
const char *ssid = "Shanu";
const char *password = "20167635";
const char *host = "api.thingspeak.com";
String apiKey = "XB7NAHQUZTG7QKS3";

WiFiClient  client;

void setup() 
{
  EEPROM.begin(512);
  lcd.begin(16, 2); 
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  ThingSpeak.begin(client);
  pinMode(D7, 0);
  digitalWrite(D7, 1);
  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  i                 = 0;
  attachInterrupt(D7, pulseCounter, FALLING); 
}

void pulseCounter()
{
  pulseCount++;
}

void LCD1()
{
  lcd.setCursor(0,0);
  lcd.print("        ");
  lcd.setCursor(0,1);
  lcd.print("        ");
  delay(1000);
  lcd.setCursor(0,0);
}

void noconn()
{
  t=EEPROM.read(1);
  t++;
  addr=2;
  while((EEPROM.read(addr)!=0)||(EEPROM.read(addr+1)!=0))
  {
    addr=addr+3;
  }
  val1=totalMilliLitres/100;
  val2=totalMilliLitres%100;
  EEPROM.write(val1,addr);
  addr++;
  EEPROM.write(val2,addr);
  addr++;
  EEPROM.write(t,addr);
  LCD1();
  lcd.print("Uploaded Offline");
}

void wifi()
{
  int b;
  LCD1();
  lcd.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    lcd.print(".");
    b++;
    if(b>10)
    {
      lcd.print("Wifi Not Found");
      delay(1000);
      noconn();
    }
  }
  lcd.print("Connected to:");
  lcd.println(ssid);
  delay(2000);
  WiFiClient client;          
  const int httpPort = 80; //Port 80 is commonly used for www
  //Connect to host, host(web site) is define at top 
  LCD1();
  lcd.print("Connecting");
  delay(1000);
  if(!client.connect(host, httpPort))
  {
    delay(500);
    lcd.print(".");
    b++;
    if(b>10)
    {
      lcd.print("No Internet");
      delay(1000);
      noconn();
    }
  }
  else                                                                                                                               // If wifi is connected
  {
    String Link;
    String ADCData;
    int t=EEPROM.read(1);
    int nt = ThingSpeak.readFloatField(Channel, 2);
    while(nt<t)
    {
      val1=EEPROM.read(addr);
      addr++;
      val2=EEPROM.read(addr);
      addr=addr+2;
      val=val1*100+val2;
      val=val/100;
      ADCData = String(val);   //String to interger conversion 
      String Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
      Link = Link + ADCData;
      Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
      client.print(Link);
      delay(100);
      ADCData = String(nt);   //String to interger conversion 
      Link="GET /update?api_key="+apiKey+"&field2=";  //Requeste webpage  
      Link = Link + ADCData;
      Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
      client.print(Link);
      nt++;
    }
    t++;
    for (int a = 0; a < 512; a++)
    {
      EEPROM.write(a, 0);
    }
    EEPROM.write(1,t);
    ADCData = String(totalMilliLitres/100);   //String to interger conversion 
    Link="GET /update?api_key="+apiKey+"&field1=";  //Requeste webpage  
    Link = Link + ADCData;
    Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
    client.print(Link);
    delay(100);
    ADCData = String(t);   //String to interger conversion 
    Link="GET /update?api_key="+apiKey+"&field2=";  //Requeste webpage  
    Link = Link + ADCData;
    Link = Link + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n";                
    client.print(Link);
    lcd.print("Uploaded Online");
    delay(1000);
    EEPROM.end();
  }  
}

void loop() 
{
  if ((millis() - oldTime) > 1000) 
  {
    LCD1();
    lcd.print("Waiting...");
    detachInterrupt(D7);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis();
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    if(totalMilliLitres!=0)
    {
      if(digitalRead(D7)==0)
      {
        i++;
      }
      if(i>10)
      {
        LCD1();
        lcd.print("Petrol Quantity: ");           
        lcd.setCursor(0,1);
        totalMilliLitres=totalMilliLitres/10;
        lcd.print(totalMilliLitres/100);
        lcd.print(".");
        lcd.print(totalMilliLitres%100);
        lcd.println(" L");
        wifi();
        i=0;
        totalMilliLitres = 0;
      }
    }
    pulseCount = 0;
    attachInterrupt(D7, pulseCounter, FALLING);
  }
}
