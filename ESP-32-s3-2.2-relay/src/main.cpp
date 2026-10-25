#include <Arduino.h>


#define TRANSISTOR_PIN 16
#define CONTROL_PIN 5


void setup() {

    pinMode(TRANSISTOR_PIN, OUTPUT);
    pinMode(CONTROL_PIN, INPUT_PULLDOWN);

    digitalWrite(TRANSISTOR_PIN, HIGH);
    Serial.begin(115200);
}

void loop() {
    uint32_t start = micros();
    digitalWrite(TRANSISTOR_PIN, LOW);
    while (!digitalRead(CONTROL_PIN)) {
    }
    uint32_t end = micros();
    Serial.println(end - start);

    digitalWrite(TRANSISTOR_PIN, HIGH);
    delay(2000);
}
