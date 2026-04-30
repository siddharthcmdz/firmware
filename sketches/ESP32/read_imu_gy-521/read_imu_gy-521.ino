#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

const int SDA_PIN = 21;
const int SCL_PIN = 22;
const float GRAVITY = 9.80665f;
const float DPS = 57.29578f;

Adafruit_MPU6050 mpu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(5000);
  Wire.begin(SDA_PIN, SCL_PIN);
  Wire.setClock(400000);

  Serial.println("==== MPU-6050 read ====");

  if(!mpu.begin()) {
    Serial.println("MPU6050 init failed! Run the I2C scanner!");
    while(1) delay(1000);
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_44_HZ);

  delay(50);

  Serial.println("Ready. Format: ax_g, ay_g, az_g, gx_dps, gy_dps, gz_dps, t_ms");
}

void loop() {
  // put your main code here, to run repeatedly:
  static unsigned long next_sample_ms = 0;
  const unsigned long SAMPLE_PERIOD_MS = 200;

  unsigned long now = millis();

  if((long)(now - next_sample_ms) < 0) return ;

  next_sample_ms = now + SAMPLE_PERIOD_MS;

  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t);

  float ax_g = a.acceleration.x / GRAVITY;
  float ay_g = a.acceleration.y / GRAVITY;
  float az_g = a.acceleration.z / GRAVITY;

   float gx_dps = g.gyro.x * DPS;
   float gy_dps = g.gyro.y * DPS;
   float gz_dps = g.gyro.z * DPS;

  Serial.printf("%.3f, %.3f, %.3f, %.2f, %.2f, %.2f, %lu\n", ax_g, ay_g, az_g, gx_dps, gy_dps, gz_dps, now);

}
