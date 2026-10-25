#include <Arduino.h>
#define RED_PIN 45
#define BLUE_PIN 48

void setup() {
    pinMode(RED_PIN, OUTPUT);
    pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
    digitalWrite(RED_PIN, HIGH);
    digitalWrite(BLUE_PIN, LOW);
    delay(200);
    digitalWrite(RED_PIN, LOW);
    digitalWrite(BLUE_PIN, HIGH);
    delay(800);
}
