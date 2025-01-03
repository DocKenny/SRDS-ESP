#include <Arduino.h>
#include <WiFi.h>
#include <MQTTPubSubClient.h>

#include "../include/config.h"

void MQTTcallback(char* topic, byte* payload, unsigned int length);
void connectToWifi();
void connectToMQTT();

WiFiClient wifiClient;
MQTTPubSubClient mqtt;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifiClient.connect(MQTT_SERVER, MQTT_PORT);
  connectToWifi();
  mqtt.begin(wifiClient);
  connectToMQTT();

}


void loop() {
  // put your main code here, to run repeatedly:
  mqtt.update();
  Serial.println("loop");
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
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void connectToMQTT() {
    Serial.print("Connecting to MQTT...");
    while (!mqtt.connect("esp32")) {  // Use a client ID here
        Serial.print(".");
        delay(1000);
    }
    Serial.println(" connected!");
}