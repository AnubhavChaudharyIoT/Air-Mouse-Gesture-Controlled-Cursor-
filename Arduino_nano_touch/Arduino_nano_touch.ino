#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
int16_t ax, ay, az, gx, gy, gz;

long gyro_offset_x, gyro_offset_y;
const int CALIBRATION_SAMPLES = 1000;

#define TOUCH_PIN 7

// Simple moving average filter
const int FILTER_SAMPLES = 10;
int gx_readings[FILTER_SAMPLES];
int gy_readings[FILTER_SAMPLES];
int filter_index = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(10);
  }

  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    while (1) {
      delay(10);
    }
  }

  mpu.setFullScaleGyroRange(MPU6050_GYRO_FS_500);

  // Calibrate the MPU6050
  calibrateMPU();

  pinMode(TOUCH_PIN, INPUT);
  
  // Initialize filter arrays
  for (int i = 0; i < FILTER_SAMPLES; i++) {
    gx_readings[i] = 0;
    gy_readings[i] = 0;
  }
}

void loop() {
  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // Apply calibration offsets
  gx -= gyro_offset_x;
  gy -= gyro_offset_y;

 // Apply moving average filter for gx, gy, gz
  gx_readings[filter_index] = gx;
  gy_readings[filter_index] = gy;
  int gz_val = gz;  // keep Z separate (optional: add filter for gz too)
  filter_index = (filter_index + 1) % FILTER_SAMPLES;

  long sum_gx = 0;
  long sum_gy = 0;
  for (int i = 0; i < FILTER_SAMPLES; i++) {
    sum_gx += gx_readings[i];
    sum_gy += gy_readings[i];
}

  int smooth_gx = sum_gx / FILTER_SAMPLES;
  int smooth_gy = sum_gy / FILTER_SAMPLES;

// Movement mapping with Z axis
  int vx = smooth_gx / 200;     // horizontal
  int vy = smooth_gy / 200;      // vertical
  int vz = gz_val / 200;         // twist → angled movement

// Combine Z rotation into X/Y for diagonal control
  vx += vz / 2;   // adjust factor for diagonal
  vy += vz / 2;   // same here

// Deadzone (optional: avoids jitter)
  if (abs(vx) < 1) vx = 0;
  if (abs(vy) < 1) vy = 0;

  int touchState = digitalRead(TOUCH_PIN);

  Serial.print(vx);
  Serial.print(",");
  Serial.print(vy);
  Serial.print(",");
  Serial.println(touchState);

  delay(10);
}

void calibrateMPU() {
  long gyro_x_sum = 0;
  long gyro_y_sum = 0;

  Serial.println("Calibrating MPU6050... Please keep the sensor still.");
  
  for (int i = 0; i < CALIBRATION_SAMPLES; i++) {
    mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    gyro_x_sum += gx;
    gyro_y_sum += gy;
    delay(2);
  }

  gyro_offset_x = gyro_x_sum / CALIBRATION_SAMPLES;
  gyro_offset_y = gyro_y_sum / CALIBRATION_SAMPLES;

  Serial.println("Calibration complete.");
  Serial.print("Gyro X Offset: ");
  Serial.println(gyro_offset_x);
  Serial.print("Gyro Y Offset: ");
  Serial.println(gyro_offset_y);
}