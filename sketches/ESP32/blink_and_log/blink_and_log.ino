const int LED_PIN = 2;
const unsigned long BLINK_PERIOD_MS = 500;
const unsigned long LOG_PERIOD_MS = 1000;

unsigned long last_blink_ms = 0;
unsigned long last_log_ms = 0;
bool led_state = false;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  Serial.println("=== RakshaBand boot ===");
  Serial.printf("Build: %s %s\n", __DATE__, __TIME__);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long now = millis();

  if (now - last_blink_ms >= BLINK_PERIOD_MS) {
    last_blink_ms = now;
    led_state = !led_state;
    digitalWrite(LED_PIN, led_state ? HIGH : LOW);
  }

  if (now - last_log_ms >= LOG_PERIOD_MS) {
    last_log_ms = now;
    Serial.printf("uptime=%lu ms, free_heap=%u bytes\n",
                  now, ESP.getFreeHeap());
  }
}
