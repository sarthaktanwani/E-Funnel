#include <SoftwareSerial.h>
#include <TinyGPS.h>
TinyGPS gps;
SoftwareSerial sgps(5, 6);

unsigned char buffer[64];
uint8_t count = 0;

void setup()
{
 Serial.begin(9600);
 sgps.begin(9600);
}
void loop()
{
  //Serial.print("Latitude :");
 while (sgps.available())
 {
  buffer[count] = sgps.read();
  count++;
  Serial.print("Ladvsb :");
   int c = sgps.read();
   if (gps.encode(c))
   {
     float slat,slon;
     gps.f_get_position(&slat, &slon);
     Serial.print("Latitude :");
     Serial.println(slat, 6);
     Serial.print("Longitude:");
     Serial.println(slon, 6);
   }
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
