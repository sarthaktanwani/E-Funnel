
#include <Sim800L.h>
#include <SoftwareSerial.h>               

#define RX  10
#define TX  11

Sim800L GSM(RX, TX);

int day,month,year,minute,second,hour;

void setup(){
  Serial.begin(9600); 
  GSM.begin(4800);   
}

void loop(){
	GSM.RTCtime(&day,&month,&year,&hour,&minute,&second);
	//NOW the variables have the real time. 
	Serial.print(day);
	Serial.print('/');
	Serial.print(month);
	Serial.print('/');
	Serial.print(year);
	Serial.print(' ');
	Serial.print(hour);
	Serial.print(':');
	Serial.print(minute);
	Serial.print(':');
	Serial.print(second);
	Serial.println();
}
