#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

#define TOUCH_PIN 7  // touch sensor digital pin

void setup() {
  Serial.begin(9600);
  
  while (!Serial) {
    delay(10);
  }

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    while (1) { delay(10); }
  }

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

  pinMode(TOUCH_PIN, INPUT);

  delay(100);
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  vx = -(gx + 260) / 150;
  vy = (gz + 100) / 150;

  // Read touch state
  int touchState = digitalRead(TOUCH_PIN);

  // Send data in format: "vx,vy,touch"
  Serial.print(vx);
  Serial.print(",");
  Serial.print(vy);
  Serial.print(",");
  Serial.println(touchState);  // 1 = touched, 0 = not touched

  delay(20);
}
