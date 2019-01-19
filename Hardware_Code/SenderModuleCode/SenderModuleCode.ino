#include "TinyGPS++.h"
#include "SoftwareSerial.h"
SoftwareSerial serial_connection(3, 5); 
TinyGPSPlus gps;
SoftwareSerial GPRS(10, 11);  //RX=pin 10, TX=pin 11
byte statusLed    = 13;

byte sensorInterrupt = 0;  // 0 = digital pin 2
byte sensorPin       = 2;

// The hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 4.5;

volatile byte pulseCount;  

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long temp;
uint8_t hr_val, min_val, sec_val;

String flon,flat,fspeed;

uint8_t flag = 0;
String message;
String destinationNumber = "+918447316525";

float lat;
float lng;
int speed1;

unsigned long oldTime;

void setup()
{
  
  // Initialize a serial connection for reporting values to the host
  Serial.begin(9600);
  GPRS.begin(9600);
  serial_connection.begin(9600);//This opens up communications to the GPS
  // Set up the status LED line as an output
  pinMode(statusLed, OUTPUT);
  digitalWrite(statusLed, HIGH);  // We have an active-low LED attached
  
  pinMode(sensorPin, INPUT);
  digitalWrite(sensorPin, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;

  // The Hall-effect sensor is connected to pin 2 which uses interrupt 0.
  // Configured to trigger on a FALLING state change (transition from HIGH
  // state to LOW state)
  attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  GPRS.println("AT");
}

/**
 * Main program loop
 */
void loop()
{
   GetLocation();
   if((millis() - oldTime) > 1000)    // Only process counters once per second
  { 
    // Disable the interrupt while calculating flow rate and sending the value to
    // the host
    detachInterrupt(sensorInterrupt);
        
    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    
    // Note the time this processing pass was executed. Note that because we've
    // disabled interrupts the millis() function won't actually be incrementing right
    // at this point, but it will still return the value it was set to just before
    // interrupts went away.
    oldTime = millis();
    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    temp = totalMilliLitres;
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    if (temp == totalMilliLitres && totalMilliLitres >= 300)
    {
      flag++;
    }
    if(flag == 10)
    {
      //if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
    //{
      lat = gps.location.lat();
      flat = String(lat, 6);
      Serial.println(lat, 6);
      lng = gps.location.lng();
      flon = String(lng, 6);
      Serial.println(lng, 6);
      speed1 = gps.speed.mph();
      fspeed = String(speed1, 6);
      Serial.println(fspeed);
      hr_val = gps.time.hour(); /* Get hour */
      min_val = gps.time.minute();  /* Get minutes */
      sec_val = gps.time.second();  /* Get seconds */
      hr_val = (hr_val + 5) % 12;
      min_val = (min_val + 30) % 60;
      String timeString = String(hr_val) + ":" + String(min_val) + ":" + String(sec_val);
    //}
      SendMessage("+918076265166", "(Quantity=542, Latitude=77.980786, Longitude=28.225148, Speed=0, Time=6:42PM)");
      //message = "(Quantity=" + (String)totalMilliLitres + ", " + "Latitude=" + flat + ", " + "Longitude=" + flon + ", " + "Speed=" + fspeed + ", " + "Time=" + timeString + ")";
      SendMessage(destinationNumber, message);
      SendMessage("+917417464147", message);
      flag = 0;
      totalMilliLitres = 0;
    }
    unsigned int frac;
    
    // Print the flow rate for this second in litres / minute
    Serial.print("Flow rate: ");
    Serial.print(int(flowRate));  // Print the integer part of the variable
    Serial.print(".");             // Print the decimal point
    // Determine the fractional part. The 10 multiplier gives us 1 decimal place.
    frac = (flowRate - int(flowRate)) * 10;
    Serial.print(frac, DEC) ;      // Print the fractional part of the variable
    Serial.print("L/min");
    // Print the number of litres flowed in this second
    Serial.print("  Current Liquid Flowing: ");             // Output separator
    Serial.print(flowMilliLitres);
    Serial.print("mL/Sec");

    // Print the cumulative total of litres flowed since starting
    Serial.print("  Output Liquid Quantity: ");             // Output separator
    Serial.print(totalMilliLitres);
    Serial.println("mL"); 

    // Reset the pulse counter so we can start incrementing again
    pulseCount = 0;
    
    // Enable the interrupt again now that we've finished sending output
    attachInterrupt(sensorInterrupt, pulseCounter, FALLING);
  }
}

/*
Insterrupt Service Routine
 */
void pulseCounter()
{
  // Increment the pulse counter
  pulseCount++;
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

void GetLocation()
{
  while(serial_connection.available())//While there are characters to come from the GPS
  {//Serial.println("Check");
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
}

