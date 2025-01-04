#include <Arduino.h>
#include <WiFi.h>
#include <MQTTPubSubClient.h>
#include <HardwareSerial.h>

#include "../include/config.h"

void MQTTcallback(char* topic, byte* payload, unsigned int length);
void connectToWifi();
void connectToMQTT();

WiFiClient wifiClient;
MQTTPubSubClient mqtt;
HardwareSerial mySerial(1);

#define RX_PIN 16  // GPIO16 for RX
#define TX_PIN 17  // GPIO17 for TX

void setup() {
  Serial.begin(115200);
  mySerial.begin(9600, SERIAL_8N1, RX_PIN, TX_PIN);
  connectToWifi();

  wifiClient.connect(MQTT_SERVER, MQTT_PORT);

  mqtt.begin(wifiClient);

  connectToMQTT();

  mqtt.subscribe([](const String& topic, const String& payload, const size_t size) {
        mySerial.println("mqtt received: " + topic + " - " + payload);
    });

  mqtt.publish("esp32", "Hello from ESP32");
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

  if(Serial.available()) {
    String message = mySerial.readString();
    mqtt.publish("esp32", message);
  }
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
