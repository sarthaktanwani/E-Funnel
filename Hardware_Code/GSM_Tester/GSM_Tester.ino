#include <SoftwareSerial.h>


//SoftwareSerial GPRS(14, 12, false, 256);
SoftwareSerial GPRS(10, 11);
String outMessage = "Message form GSM Module";
String destinationNumber = "+917417464147";

unsigned char buffer[64];
uint8_t count = 0;
void setup() {
  // put your setup code here, to run once:
  GPRS.begin(9600);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (GPRS.available())
  {
    buffer[count] = GPRS.read();
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
  if (Serial.available())
  {
    byte b = Serial.read();
    if (b == '*')
      GPRS.write( 0x1a );
    else if (b == 's')
      SendMessage("+919810004136", "(Message is: hackzor)");
    /*else if (b == 'e')
      showData();
    else if (b == 'r')
      resetData();*/
    else
      GPRS.write(b);
  }
}

void clearbufferArray()
{
  for (uint8_t i = 0; i < count; i++)
  {
    buffer[i] = NULL;
  }
}

void SendMessage(String Number, String Message)
{
  Serial.println("Sending message!");
  GPRS.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);  // Delay of 1000 milli seconds or 1 second
  GPRS.println("AT+CMGS=\"" + Number + "\"\r"); // Replace x with mobile number
  //GPRS.println("AT+CMGS=\"+919910004136\"\r");
  delay(1000);
  GPRS.println(Message);// The SMS text you want to send
  delay(1000);
  GPRS.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  Serial.println("Message Sent!");
}
