#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#include "../include/config.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin("SSID", "PASSWORD");

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println("Connected to the WiFi network");


}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Hello World!");
}
