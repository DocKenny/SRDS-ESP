#include <Arduino.h>
#include <WiFi.h>
#include <MQTTPubSubClient.h>

#include "../include/config.h"

void MQTTcallback(char* topic, byte* payload, unsigned int length);

WiFiClient wifiClient;
MQTTPubSubClient mqtt;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");

  while(!espClient.connected()) {

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