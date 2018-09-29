float calibrationFactor = 4.5;
volatile byte pulseCount;
int i;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long oldTime;
String message;

void pulseCounter()
{
  pulseCount++;
  Serial.println(pulseCount);
}

void setup()
{
  Serial.begin(9600);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  pulseCount        = 0;
  flowRate          = 0.0;
  flowMilliLitres   = 0;
  totalMilliLitres  = 0;
  oldTime           = 0;
  i                 = 0;
  attachInterrupt(0, pulseCounter, FALLING); // Configured to trigger on a FALLING state change (transition from HIGH state to LOW state)
}

void loop()
{
  if ((millis() - oldTime) > 1000)   // Only process ounters once per second
  {
    detachInterrupt(0);
    flowRate = ((1000.0 / (millis() - oldTime)) * pulseCount) / calibrationFactor;
    oldTime = millis(); 
    flowMilliLitres = (flowRate / 60) * 1000;
    totalMilliLitres += flowMilliLitres;
    if(totalMilliLitres!=0)
    {
      if(digitalRead(2)==0)
      {
        i++;
      }
      if(i>10)
      {
        Serial.print("  Output Liquid Quantity: ");
        Serial.print(totalMilliLitres);                                             //Value taken from here                       
        Serial.println("mL");
        i=0;
        totalMilliLitres = 0;
      }
    }
    pulseCount = 0;
    attachInterrupt(0, pulseCounter, FALLING);
  }
}
