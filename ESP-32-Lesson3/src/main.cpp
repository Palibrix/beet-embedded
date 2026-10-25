#include <Arduino.h>

#define BUTTON_PIN 16

volatile uint32_t counter = 0;
uint32_t last_counter = 0;

void IRAM_ATTR perfom_count() {
    counter++;
}

void setup() {
    pinMode(BUTTON_PIN, INPUT);
    Serial.begin(9600);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), perfom_count, FALLING);
}

void loop() {
    if (counter != last_counter) {
        last_counter = counter;
        Serial.println(counter);
    }
    // Serial.println(counter);
    delay(10);
}

