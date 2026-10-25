#include <Arduino.h>

#define BUTTON_1_PIN 15
#define BUTTON_2_PIN 0
#define LED_1_PIN 16
#define LED_2_PIN 17
unsigned int mode = 1;
unsigned int led_state = 0;

void setup() {
    Serial.begin(9600);
    pinMode(BUTTON_1_PIN, INPUT);
    pinMode(BUTTON_2_PIN, INPUT);

    pinMode(LED_1_PIN, OUTPUT);
    pinMode(LED_2_PIN, OUTPUT);

    delay(1000);
}

void loop() {
    int mode_1 = digitalRead(BUTTON_1_PIN);
    int mode_2 = digitalRead(BUTTON_2_PIN);
    Serial.println("Mode 1: " + String(mode_1) + "; Mode 2: " + String(mode_2));
    if (mode_1 == LOW && mode == 2) {
        mode = 1;
        Serial.println("Changed to mode 1");
    } else if (mode_2 == LOW && mode == 1) {
        mode = 2;
        Serial.println("Changed to mode 2");
    }
    unsigned int delayTime = mode == 1 ? 200 : 1000;
    switch (mode) {
        case 1:
            digitalWrite(LED_1_PIN, led_state);
            digitalWrite(LED_2_PIN, led_state);
            led_state = !led_state;
            break;
        case 2:
            digitalWrite(LED_1_PIN, led_state);
            digitalWrite(LED_2_PIN, !led_state);
            led_state = !led_state;
            break;
        default:
            digitalWrite(LED_1_PIN, LOW);
            digitalWrite(LED_2_PIN, LOW);
    }

    delay(delayTime);
}