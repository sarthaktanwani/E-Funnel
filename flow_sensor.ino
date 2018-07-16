/*
 * This code will give the net volume of liquid passed through flow sensor with or without break (less than 10 sec.). 
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
 *       2)      Hall Effect Sensor YF-S 201
 *                                        Data Pin           =                    D7
 *                
 *                                        
 *                --"MADE BY AYUSH AGGARWAL"
 */
#include <LiquidCrystal.h>
LiquidCrystal lcd(D0, D1, D2, D3, D4, D5);

float calibrationFactor = 4.5;
volatile byte pulseCount; 
int i;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;


void LCD1()
{
  lcd.setCursor(0,0);
  lcd.print("        ");
  lcd.setCursor(0,1);
  lcd.print("        ");
  delay(1000);
  lcd.setCursor(0,0);
}

void setup()
{
  Serial.begin(115200);
  lcd.begin(16, 2);
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

void loop()
{
  digitalWrite(D4,0);
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
        lcd.print(totalMilliLitres/1000);
        lcd.print(".");
        lcd.print(totalMilliLitres%1000);
        lcd.println(" L");
        i=0;
        totalMilliLitres = 0;
      }
    }
    pulseCount = 0;
    attachInterrupt(D7, pulseCounter, FALLING);
  }
}

void pulseCounter()
{
  pulseCount++;
}
