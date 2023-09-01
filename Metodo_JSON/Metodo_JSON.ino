#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <MPU6050.h>
#include <ArduinoJson.h> // Include the ArduinoJson library

const char *ssid = "AndroidAP618B";
const char *password = "doip8189";
const int serverPort = 8080; // Change this to the port your server is running on
const char *serverAddress = "192.168.74.107"; // Change this to your server's IP address

MPU6050 mpu;

struct Vector3f {
  float x;
  float y;
  float z;
};

Vector3f lastAccel;
Vector3f lastGyro;

const float movementThreshold = 0.1; // Adjust this threshold as needed

void setup() {
  Wire.begin();
  mpu.initialize();
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("WiFi connected");

  Serial.println("Configuring HTTP server...");
}

void loop() {
  int16_t accelerometer_x, accelerometer_y, accelerometer_z;
  int16_t gyro_x, gyro_y, gyro_z;

  accelerometer_x = mpu.getAccelerationX();
  accelerometer_y = mpu.getAccelerationY();
  accelerometer_z = mpu.getAccelerationZ();
  
  gyro_x = mpu.getRotationX();
  gyro_y = mpu.getRotationY();
  gyro_z = mpu.getRotationZ();

  // Create a JSON document
  StaticJsonDocument<200> jsonDocument;
  jsonDocument[0] = accelerometer_x + ", ";
  jsonDocument[1] = accelerometer_y + ", ";
  jsonDocument[2] = accelerometer_z + " _ ";
  jsonDocument[3] = gyro_x + ", ";
  jsonDocument[4] = gyro_y + ", ";
  jsonDocument[5] = gyro_z;

  // Serialize the JSON document to a string
  String jsonData;
  serializeJson(jsonDocument, jsonData);

  Serial.println(jsonData);

  Serial.println("Sending data to the server...");

  HTTPClient http;
  WiFiClient client;
  http.begin(client, "http://" + String(serverAddress) + ":" + String(serverPort));
  http.addHeader("Content-Type", "application/json");

  // Send the JSON data to the server
  int httpCode = http.POST(jsonData);

  if (httpCode == HTTP_CODE_OK) {
    String response = http.getString();
    Serial.println("Server Response:");
    Serial.println(response);
  } else {
    Serial.print("HTTP Connection Error: ");
    Serial.println(httpCode);
  }

  http.end();

  delay(500);
  
}

// hwihdiwhdwihdwod
