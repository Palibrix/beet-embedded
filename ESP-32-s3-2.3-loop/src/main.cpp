#include <Arduino.h>

struct LED {
    uint8_t pin;
    uint16_t delay;
    unsigned long previousMillis;
};

static LED led1 = {.pin = 15, .delay = 200};
static LED led2 = {.pin = 16, .delay = 500};
static LED led3 = {.pin = 17, .delay = 1000};

static void check_led(LED &led, unsigned long currentMillis);

void setup() {
    pinMode(led1.pin, OUTPUT);
    pinMode(led2.pin, OUTPUT);
    pinMode(led3.pin, OUTPUT);
}

void loop() {
    unsigned long currentMillis = millis();
    check_led(led1, currentMillis);
    check_led(led2, currentMillis);
    check_led(led3, currentMillis);
}

void check_led(LED &led, unsigned long currentMillis) {
    if (currentMillis - led.previousMillis > led.delay) {
        digitalWrite(led.pin, !digitalRead(led.pin));
        led.previousMillis = currentMillis;
    }
}