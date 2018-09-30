#define MaxNumbersLength  15
#define MaxDateTimeLength 22
#define MaxMessageLength  50
#include "SoftwareSerial.h"
#include <ESP8266WiFi.h>

String apiWritekey = "3DGEBMEGUHCHM0TQ"; // replace with your THINGSPEAK WRITEAPI key here
const char* ssid = "Shanu1"; // your wifi SSID name
const char* password = "20167635" ;// wifi pasword
 
const char* server = "api.thingspeak.com";
float resolution=3.3/1023;// 3.3 is the supply volt  & 1023 is max analog read value
WiFiClient client;

SoftwareSerial GPRS(14, 12, false, 256);    //Syntax: SoftwareSerial::SoftwareSerial(int receivePin, int transmitPin, bool inverse_logic, unsigned int buffSize)
                                            // GPIO14 -> D5 pin, GPIO12 -> D6 pin on the NodeMCU

uint8_t NoOfQuotes = 0;

unsigned char buffer[64];
String Number1 = "";
String DateTime1 = "";
String RcvdMessage1 = "";
float Quality;
float Quantity;
float Latitude;
float Longitude;

uint8_t count = 0;
bool MsgStartFlag = 0;
bool flag = 0;
void setup() {
  // put your setup code here, to run once:
  GPRS.begin(9600);
  Serial.begin(9600);
  WiFi.disconnect();
  delay(10);
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("NodeMcu connected to wifi...");
  Serial.println(ssid);
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(GPRS.available() && flag == 0)
    {
      Serial.println("Receiving Message! ....... by Receiver Module");
      flag = 1;
    }
  while (GPRS.available())
  {
    buffer[count] = GPRS.read();
    if( NoOfQuotes == 1 && buffer[count] != '"')
      {
        //Number[numbercount++] = buffer[count];
        Number1 += (char)buffer[count];
      }
    if( NoOfQuotes == 5 && buffer[count] != '"')
      {
        //DateTime[datetimecount++] = buffer[count];
        DateTime1 += (char)buffer[count];
      }
    if( buffer[count] == ')')
      {
        MsgStartFlag = 0;
        flag = 0;
        showData();
        resetData();
      }
    if( MsgStartFlag == 1)
      {
        //RcvdMessage[messagecount++] = buffer[count];
        RcvdMessage1 += (char)buffer[count];
      }
    if( buffer[count] == '(')
      MsgStartFlag = 1;
    if(buffer[count] == '"')
      NoOfQuotes++;
    if (count == 64)
      {
        Serial.println("BREAKING!!");
        break;
      }
      count++;
  }
  
  Serial.write(buffer, count);
  clearbufferArray();
  count = 0;
}

void clearbufferArray()
{
  for (uint8_t i = 0; i < count; i++)
  {
    buffer[i] = NULL;
  }
}

void showData()
{
    /*Serial.println();
    Serial.print("Number is: ");
    Serial.write(Number, numbercount);
    Serial.println();
    Serial.print("Date and Time are: ");
    Serial.write(DateTime, datetimecount);
    Serial.println();
    Serial.print("Message is: ");
    Serial.write(RcvdMessage, messagecount);
    Serial.println();
    Serial.print("No of Double Quotes: ");
    Serial.println(NoOfQuotes);*/
    Serial.println();
    Serial.print("Number1 is: ");
    Serial.print(Number1);
    Serial.println();
    Serial.print("DateTime1 is: ");
    Serial.print(DateTime1);
    Serial.println();
    Serial.print("Message1 is: ");
    Serial.print(RcvdMessage1);
    Serial.println();
    myString = RcvdMessage1;
    Serial.print("Quality is: ");
    Quality = (myString.substring(myString.indexOf("Quality=") + strlen("Quality="),myString.indexOf(","))).toFloat();
    Serial.println(Quality, 5);
    myString.remove(0, myString.indexOf("Quantity="));
    Serial.print("Quantity is: ");
    Quantity = (myString.substring(myString.indexOf("Quantity=") + strlen("Quantity="),myString.indexOf(","))).toInt();
    Serial.println(Quantity);
    myString.remove(0, myString.indexOf("Latitude="));
    Serial.print("Latitude is: ");
    Latitude = (myString.substring(myString.indexOf("Latitude=") + 9,myString.indexOf(","))).toFloat();
    Serial.println(Latitude, 6);
    myString.remove(0, myString.indexOf("Longitude="));
    Serial.print("Longitude is: ");
    Longitude = (myString.substring(myString.indexOf("Longitude=") + 10)).toFloat();
    Serial.println(Longitude, 6);
    //SendThingspeak(Quality, Quantity, Latitude, Longitude);
    SendWebhost(Quality, Quantity, Latitude, Longitude);
}

void SendThingspeak(float Quality, float Quantity, float Latitude, float Longitude)
{
  if (client.connect(server,80))
  {  
    String tsData = apiWritekey;
           tsData +="&field1=";
           tsData += String(Quality);
           tsData +="&field2=";
           tsData += String(Quantity);
           tsData +="&field3=";
           tsData += String(Latitude);
           tsData +="&field4=";
           tsData += String(Longitude);
           tsData += "\r\n\r\n";
 
     client.print("POST /update HTTP/1.1\n");
     client.print("Host: api.thingspeak.com\n");
     client.print("Connection: close\n");
     client.print("X-THINGSPEAKAPIKEY: "+apiWritekey+"\n");
     client.print("Content-Type: application/x-www-form-urlencoded\n");
     client.print("Content-Length: ");
     client.print(tsData.length());
     client.print("\n\n");  // the 2 carriage returns indicate closing of Header fields & starting of data
     client.print(tsData);
     Serial.print("Quality is: ");
     Serial.println(Quality, 5);
     Serial.print("Quantity is: ");
     Serial.println(Quantity, 5);
     Serial.print("Latitude is: ");
     Serial.println(Latitude, 6);
     Serial.print("Longitude is: ");
     Serial.println(Longitude, 6);
     Serial.println("uploaded to Thingspeak server....");
  }
  client.stop();
 
  Serial.println("Waiting to upload next reading...");
  Serial.println();
  // thingspeak needs minimum 15 sec delay between updates
  delay(15000);
}

void resetData()
{
    Number1 = "";
    DateTime1 = "";
    RcvdMessage1 = "";
    NoOfQuotes    = 0;
    //numbercount   = 0;
    //datetimecount = 0;
    //messagecount  = 0;
}


