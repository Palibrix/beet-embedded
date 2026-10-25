#include <Arduino.h>

#define TRANSISTOR_PIN 16

void setup() {
    pinMode(TRANSISTOR_PIN, OUTPUT);
}

void loop() {
    digitalWrite(TRANSISTOR_PIN, HIGH);
}