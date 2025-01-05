#include <Arduino.h>
#include <WiFi.h>
#include <MQTTPubSubClient.h>
#include <HardwareSerial.h>
#include <Wire.h>
#include "Adafruit_VL53L0X.h"

#include "../include/config.h"

void MQTTcallback(char* topic, byte* payload, unsigned int length);
void connectToWifi();
void connectToMQTT();

WiFiClient wifiClient;
MQTTPubSubClient mqtt;
HardwareSerial mySerial(1);
// Create an instance of the sensor
Adafruit_VL53L0X tof = Adafruit_VL53L0X();

#define RX_PIN 16 
#define TX_PIN 17  
#define I2C_SDA 20
#define I2C_SCL 21

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  mySerial.println("test");
  Wire.begin(I2C_SDA, I2C_SCL);

   // Initialize the sensor with the new I2C address
  if (!tof.begin(0x29)) {
    mySerial.println("Failed to find the sensor");
    while (1);  // Halt execution if sensor is not found
  }

  mySerial.println("Sensor Initialized!");

  connectToWifi();

  wifiClient.connect(MQTT_SERVER, MQTT_PORT);

  mqtt.begin(wifiClient);

  connectToMQTT();

  mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        mySerial.println("mqtt received: " + topic + " - " + payload);
    });

  mqtt.publish("esp32", "Hello from ESP32");
  

  mySerial.println("test");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }

  if (!mqtt.isConnected()) {
    connectToMQTT();
  }

  mqtt.update();
  mySerial.println("loop");
  VL53L0X_RangingMeasurementData_t measure;

  // Read the sensor data
  tof.rangingTest(&measure, false);
  
  // Display the distance measurement
  if (measure.RangeStatus != 4) {  // 4 means out of range
    Serial.print("Distance (mm): ");
    Serial.println(measure.RangeMilliMeter);
  } else {
    Serial.println("Out of range");
  }
  
  delay(100);  
}

void MQTTcallback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void connectToWifi() {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");
}

void connectToMQTT() {
    Serial.print("Connecting to MQTT...");
    while (!mqtt.connect("esp32")) {  // Use a client ID here
        Serial.print(".,");
        delay(1000);
    }
    Serial.println(" connected!");
}
