#include <Arduino.h>
#define BUTTON_PIN 15
#define CHECK_EVERY_MS 10

enum STATE {
    IDLE,
    PRESSED_DEBOUNCE,
    PRESSED,
    RELEASED_DEBOUNCE,
};


static STATE currentState = IDLE;
static uint8_t buttonCounter = 0;
static uint32_t lastCheckMs = 0;


void setup() {
    pinMode(BUTTON_PIN, INPUT_PULLDOWN);
    Serial.begin(9600);
}


void loop() {
    uint32_t currentMs = millis();
    if (currentMs - lastCheckMs >= CHECK_EVERY_MS) {
        lastCheckMs = currentMs;
        boolean buttonLevel = digitalRead(BUTTON_PIN);

        switch (currentState) {
            case IDLE:
                if (buttonLevel == HIGH) currentState = PRESSED_DEBOUNCE;
                break;
            case PRESSED_DEBOUNCE:
                if (buttonLevel == HIGH) {
                    currentState = PRESSED;
                    buttonCounter++;
                    Serial.println(buttonCounter);
                }
                else currentState = IDLE;
                break;
            case PRESSED:
                if (buttonLevel == LOW) currentState = RELEASED_DEBOUNCE;
                break;
            case RELEASED_DEBOUNCE:
                if (buttonLevel == LOW) currentState = IDLE;
                else currentState = PRESSED;
                break;

            default:
                break;
        }

    }

}