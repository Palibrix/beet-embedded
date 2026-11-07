#include "display_helper.h"

void setupDisplay() {
    for (uint8_t pin: ATTEMPT_LEDS) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
}

void resetLeds() {
    for (uint8_t pin: ATTEMPT_LEDS) {
        digitalWrite(pin, LOW);
    }
}