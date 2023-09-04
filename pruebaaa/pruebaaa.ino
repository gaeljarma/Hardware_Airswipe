#include <ArduinoJson.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

const int numDataPoints = 10; // Número de lecturas que se realizarán
const int delayBetweenReadings = 50; // Retardo entre lecturas en milisegundos

void setup() {
  Wire.begin();
  mpu.initialize();
  Serial.begin(115200);
}

void loop() {
  unsigned long startTime = millis();
  
  // Recopilar datos durante medio segundo
  String dataLine;
  for (int i = 0; i < numDataPoints; i++) {
    int16_t accelerometer_x, accelerometer_y, accelerometer_z;
    int16_t gyro_x, gyro_y, gyro_z;

    accelerometer_x = mpu.getAccelerationX();
    accelerometer_y = mpu.getAccelerationY();
    accelerometer_z = mpu.getAccelerationZ();

    gyro_x = mpu.getRotationX();
    gyro_y = mpu.getRotationY();
    gyro_z = mpu.getRotationZ();

    String data = String(accelerometer_x) + "," +
                  String(accelerometer_y) + "," +
                  String(accelerometer_z) + "," +
                  String(gyro_x) + "," +
                  String(gyro_y) + "," +
                  String(gyro_z);

    dataLine += data;
    if (i < numDataPoints - 1) {
      dataLine += ",";
    }
    
    delay(delayBetweenReadings);
  }   
  
  Serial.println(dataLine); // Enviar toda la línea de datos
  
  delay(10); // Esperar 4 segundos antes de la próxima recopilación
}
