#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const int SDA_PIN = 21; // your Elegoo ESP32's I²C pins
const int SCL_PIN = 22;
const int LED_PIN = 2; // onboard LED pin (optional, for status indication)

Adafruit_MPU6050 mpu;

const float FREEFALL_THRESHOLD_G = 0.4f; // Threshold in m/s^2 to detect free fall
const int FREEFALL_MIN_SAMPLES = 10;     // Minimum number of consecutive samples below threshold to confirm free fall
const float IMPACT_THRESHOLD_G = 2.0f;   // Threshold in m/s^2 to detect impact
const int IMPACT_WINDOW_SAMPLES = 50;    // Number of samples to check for impact after free fall
const int POST_FALL_HOLD_SAMPLES = 200;  // Number of samples to hold after detecting free fall

enum FallState
{
  IDLE,
  IN_FREEFALL,
  AWAITING_FOR_IMPACT,
  FALL_DETECTED
};

FallState state = IDLE;
int freefall_run = 0;
int impact_window_remaining = 0;
int post_fall_remaining = 0;

bool identifyMPU6050()
{
  Wire.beginTransmission(0x68); // MPU6050 I2C address
  Wire.write(0x75);             // WHO_AM_I register
  Wire.endTransmission(false);

  Wire.requestFrom(0x68, 1); // Request 1 byte from the MPU6050
  if (Wire.available())
  {
    uint8_t whoAmI = Wire.read();
    return whoAmI == 0x68; // Check if the response matches the expected value
  }

  return false; // If no response, return false
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(460800);
  pinMode(LED_PIN, OUTPUT);
  delay(500); // Wait for Serial to initialize
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  bool mpu6050Detected = identifyMPU6050();
  if (mpu6050Detected)
  {
    Serial.println("MPU6050 detected!");
  }
  else
  {
    Serial.println("MPU6050 not detected.");
    while (1)
      delay(1000); // Stop execution if MPU6050 is not detected
  }

  if (!mpu.begin())
  {
    Serial.println("Failed to find MPU6050 chip");
    while (1)
      delay(1000); // Stop execution if MPU6050 initialization fails
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  // Serial.println("Ready. Format: AccelX, AccelY, AccelZ, GyroX, GyroY, GyroZ, ms");
}

const float G_TO_MS2 = 9.80665f; // Conversion factor from g to m/s^2
void loop()
{
  // put your main code here, to run repeatedly:
  static unsigned long next_sample_ms = 0;
  const unsigned long sample_interval_ms = 10; // Sample every 100 ms

  unsigned long now = millis();
  if ((long)(now - next_sample_ms) < 0)
  {
    return; // Not time for the next sample yet
  }
  next_sample_ms += sample_interval_ms;

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  float accelX = a.acceleration.x / G_TO_MS2; // Convert to g for easier threshold comparison
  float accelY = a.acceleration.y / G_TO_MS2;
  float accelZ = a.acceleration.z / G_TO_MS2;

  float accelMagnitude = sqrt(accelX * accelX + accelY * accelY + accelZ * accelZ);

  switch (state)
  {
  case IDLE:
  {
    if (accelMagnitude < FREEFALL_THRESHOLD_G)
    {
      freefall_run = 1;
      state = IN_FREEFALL;
    }
  }
  break;

  case IN_FREEFALL:
  {
    if (accelMagnitude < FREEFALL_THRESHOLD_G)
    {
      freefall_run++;
      if (freefall_run >= FREEFALL_MIN_SAMPLES)
      {
        state = AWAITING_FOR_IMPACT;
        impact_window_remaining = IMPACT_WINDOW_SAMPLES;
        Serial.println("Free fall detected, awaiting impact...");
      }
    }
    else
    {
      freefall_run = 0; // Reset if we go back above threshold
      state = IDLE;
    }
  }
  break;

  case AWAITING_FOR_IMPACT:
  {
    if (accelMagnitude > IMPACT_THRESHOLD_G)
    {
      state = FALL_DETECTED;
      post_fall_remaining = POST_FALL_HOLD_SAMPLES;
      digitalWrite(LED_PIN, HIGH); // Turn on LED to indicate fall detected
      // Serial.printf("Fall candidate detected, peak ~%0.2fg\n", accelMagnitude);
    }
    else
    {
      impact_window_remaining--;
      if (impact_window_remaining <= 0)
      {
        state = IDLE; // No impact detected within window, reset
        freefall_run = 0;
        // Serial.println("No impact detected, resetting...");
      }
    }
  }
  break;

  case FALL_DETECTED:
  {
    post_fall_remaining--;
    if (post_fall_remaining <= 0)
    {
      state = IDLE; // Reset after hold period
      freefall_run = 0;
      digitalWrite(LED_PIN, LOW); // Turn off LED
      // Serial.println("Resetting after fall detection...");
    }
  }
  break;
  default:
    break;
  }

  // Serial.printf("%.3f,%.3f,%.3f\n", accelX, accelY, accelZ);

  float gyroX = g.gyro.x * 57.29578f;
  float gyroY = g.gyro.y * 57.29578f;
  float gyroZ = g.gyro.z * 57.29578f;

  Serial.printf("%lu, %.4f,%.4f,%.4f,%.3f,%.3f,%.3f\n",
                now,
                accelX, accelY, accelZ,
                gyroX, gyroY, gyroZ);
}
