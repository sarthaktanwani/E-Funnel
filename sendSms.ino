#include <Sim800L.h>
#include <SoftwareSerial.h>               

#define RX  10
#define TX  11

Sim800L GSM(RX, TX);
char* text;
char* number;
bool error; 					//to catch the response of sendSms


void setup(){
	GSM.begin(4800); 			
	text="Testing Sms";  		//text for the message. 
	number="+918447316525"; 		//change to a valid number.
	error=GSM.sendSms(number,text);
}

void loop(){
	//do nothing
}
