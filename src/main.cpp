#include <Arduino.h>
#include <WiFi.h>
#include <MQTTPubSubClient.h>

#include "../include/config.h"

void MQTTcallback(char* topic, byte* payload, unsigned int length);
void connectToWifi();
void connectToMqtt();

WiFiClient wifiClient;
MQTTPubSubClient mqtt;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  connectToWifi();

  connectToMqtt();

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");

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