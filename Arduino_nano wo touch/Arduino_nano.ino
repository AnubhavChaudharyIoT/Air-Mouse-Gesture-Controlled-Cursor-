#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;
int vx, vy;

void setup() {
  Serial.begin(9600);
  
  // Wait for serial port to connect
  while (!Serial) {
    delay(10);
  }
  
  // Initialize I2C
  Wire.begin();
  
  // Initialize MPU6050
  mpu.initialize();
  if (!mpu.testConnection()) {
    while (1) {
      delay(10); // Wait indefinitely if MPU6050 is not connected
    }
  }
  
  // Optional: Set gyro range (default is +/- 250 deg/s)
  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);
  
  delay(100); // Short delay to stabilize
}

void loop() {
  // Read raw data from MPU6050
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // Calculate mouse movement from gyroscope data
  vx = -(gx + 260) / 150; // Adjust gx offset and sensitivity
  vy = (gz + 100) / 150;  // Adjust gz offset and sensitivity

  // Send data over serial: "vx,vy"
  Serial.print(vx);
  Serial.print(",");
  Serial.println(vy);

  delay(20); // Control update rate (50 Hz)
}