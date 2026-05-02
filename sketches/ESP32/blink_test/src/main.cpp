#include <Arduino.h>

const int LED_PIN = 2;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(115200);
  delay(500); // Wait for Serial to initialize
  Serial.println("ESP32 Blink Test");
  Serial.printf("Build: %s %s", __DATE__, __TIME__);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, HIGH);
  Serial.println("LED on");
  delay(1000);
  digitalWrite(LED_PIN, LOW);
  Serial.println("LED off");
  delay(1000);
}

