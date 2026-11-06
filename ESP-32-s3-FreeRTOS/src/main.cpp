#include <Arduino.h>

#define LED_PIN1 4 // 200
#define LED_PIN2 6 // 500
#define LED_PIN3 15 // 1000

struct LedConfig {
    const char *name;
    int pin;
    int delay;
};

static const LedConfig LEDS[] = {
    {.name="LED1", .pin = LED_PIN1, .delay = 200},
    {.name = "LED2", .pin = LED_PIN2, .delay = 500},
    {.name = "LED3", .pin = LED_PIN3, .delay = 1000}
};


void blinkLed(void *pvParams) {
    auto led = (LedConfig *)pvParams;
    bool state = false;
    while (1) {
        digitalWrite(led->pin, state);
        state = !state;
        vTaskDelay(pdMS_TO_TICKS(led->delay));
    }
}

void setup() {
    for (const LedConfig &led : LEDS) {
        pinMode(led.pin, OUTPUT);
        xTaskCreate(blinkLed, led.name, 2048, (void *)&led, 1, NULL);
    }
}

void loop() {
// write your code here
}