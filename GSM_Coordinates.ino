#include <Sim800L.h>
#include <SoftwareSerial.h>

#define RX  10
#define TX  11

Sim800L GSM(RX, TX);

void setup() {
  Serial.begin(9600);
  GSM.begin(9600);
}

void loop() {
if (GSM.calculateLocation()) Serial.println("Location Calculated");
  else Serial.println("Error in calculating location");

  Serial.print("Location Code: ");
  Serial.println(GSM.getLocationCode());

  Serial.print("Longitude: ");
  Serial.println(GSM.getLongitude());

  Serial.print("Latitude: ");
  Serial.println(GSM.getLatitude());
  delay(1000);
}
