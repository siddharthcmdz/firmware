void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  Serial.println("=== RakshaBand boot ===");
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long counter = 0;
  Serial.print("tick");
  Serial.println(counter++);
  delay(1000);
}
