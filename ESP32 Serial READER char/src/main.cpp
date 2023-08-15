#include <Arduino.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Initialized");
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
      String receivedString = {Serial.readStringUntil('\n')};
      Serial.println(receivedString);
  }
}