#define MaxNumbersLength  15
#define MaxDateTimeLength 22
#define MaxMessageLength  50
#include "SoftwareSerial.h"
//SoftwareSerial GPRS(9, 10);
SoftwareSerial GPRS(5, 6);

String outMessage = "Message form GSM Module";
String destinationNumber = "+917417464147";

uint8_t NoOfQuotes = 0;
//uint8_t NoOfEndLines = 0;

unsigned char buffer[64];
//char Number[MaxNumbersLength];
//char DateTime[MaxDateTimeLength];
//char RcvdMessage[MaxMessageLength];
String Number1 = "";
String DateTime1 = "";
String RcvdMessage1 = "";

//uint8_t numbercount   = 0;
//uint8_t datetimecount = 0;
//uint8_t messagecount  = 0;
//uint8_t incoming;
uint8_t count = 0;
bool MsgStartFlag = 0;
bool flag = 0;
void setup() {
  pinMode(7, INPUT);
  //delay(20000);
  // put your setup code here, to run once:
  //GPRS.begin(9600);
  GPRS.begin(9600);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(GPRS.available() && flag == 0)
    {
      Serial.println("Receiving Message!");
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
  /*for (uint8_t i = 0; i < count; i++)
  {
    Serial.print(" ");
    Serial.print(buffer[i]);
    //if(i == (count-1))
      //Serial.println();
  }*/
  Serial.write(buffer, count);
  clearbufferArray();
  count = 0;
  //Serial.println(digitalRead(7));
  if(digitalRead(7))
  {
    SendMessage("+919910004136", "(Hello Sarthak!)");
  }
  if (Serial.available())
  {
    byte b = Serial.read();
    if (b == '*')
      GPRS.write( 0x1a );
    else if (b == 's')
      SendMessage("+919810004136", "(Message is: hackzor)");
    else if (b == 'e')
      showData();
    else if (b == 'r')
      resetData();
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
  Serial.println("Sending message! ...... From Sender Module");
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
    Serial.println(RcvdMessage1);
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
