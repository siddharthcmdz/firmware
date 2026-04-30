#include <Wire.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Wire.begin(21, 22);
  delay(1000);

  Serial.println("Scanning for I2C devices..");

  for(byte addr = 8; addr < 127; addr++) {
    Wire.beginTransmission(addr);
    if(Wire.endTransmission() == 0) {
      Serial.print("Device found at : 0x");
      Serial.println(addr, HEX);
    }
  }
  Serial.println("Done");
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Executing something");
  delay(2000);
}
