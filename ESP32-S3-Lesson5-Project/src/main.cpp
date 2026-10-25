#include <Arduino.h>

#define POTENTIOMETER_PIN 3
#define PHOTORESISTOR_PIN 8
#define BUTTON_PIN 18
#define MAIN_LED_VAL 255
#define SIDE_LED_VAL 25

constexpr int RED_LED_PINS[]{4, 5, 6, 7};
constexpr int NUM_LEDS = sizeof(RED_LED_PINS) / sizeof(RED_LED_PINS[0]);
volatile uint8_t LED_WAVE_SIZE = NUM_LEDS + 3;

enum Mode {
    WAVE,
    WAVE_REVERSE,
    WAVE_BOUNCE,
    NUM_MODES
};

volatile uint8_t main_led_idx = 0;
volatile Mode current_mode = WAVE;
volatile unsigned long last_interrupt_time = 0;
volatile bool bounce_reverse = true;

void changeMode();

void changeType();

int getNextIdx();

int getPrevIdx();

void writeLed(uint8_t idx, uint8_t val);

void runWave(bool reverse = false, bool is_analog = true);

void runBouncyWave();

void switchMainLed(bool reverse);

void setup() {
    Serial.begin(9600);
    analogReadResolution(10);
    analogWriteResolution(8);
    pinMode(BUTTON_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), changeMode, FALLING);

    for (int i: RED_LED_PINS) {
        pinMode(i, OUTPUT);
    }
    delay(100);
}

void changeMode() {
    unsigned long current_time = millis();

    if (current_time - last_interrupt_time > 500) {
        current_mode = static_cast<Mode>((current_mode + 1) % NUM_MODES);
        last_interrupt_time = current_time;
    }
}


void loop() {
    uint32_t wave_speed = analogRead(POTENTIOMETER_PIN);
    wave_speed = (wave_speed + 25) * 2;
    uint32_t type_raw = analogRead(PHOTORESISTOR_PIN);

    bool is_analog = type_raw < 600;

    Serial.println("Current mode: " + String(current_mode) + " " + is_analog);

    switch (current_mode) {
        case WAVE:
            runWave(false, is_analog);
            break;

        case WAVE_REVERSE:
            runWave(true, is_analog);
            break;

        case WAVE_BOUNCE:
            runBouncyWave();
            break;

        default:
            break;
    }

    delay(wave_speed);
    for (int led: RED_LED_PINS) {
        analogWrite(led, 0);
    }
}

void runWave(bool reverse, bool is_analog) {
    LED_WAVE_SIZE = NUM_LEDS + 1;
    writeLed(main_led_idx, MAIN_LED_VAL);

    if (is_analog) {
        LED_WAVE_SIZE = NUM_LEDS + 3;
        uint8_t next_led_idx = getNextIdx();
        uint8_t prev_led_idx = getPrevIdx();
        writeLed(next_led_idx, SIDE_LED_VAL);
        writeLed(prev_led_idx, SIDE_LED_VAL);
    }

    switchMainLed(reverse);
}

void runBouncyWave() {
    LED_WAVE_SIZE = NUM_LEDS;
    writeLed(main_led_idx, MAIN_LED_VAL);
    if (main_led_idx >= LED_WAVE_SIZE - 1) {
        bounce_reverse = true;
        switchMainLed(true);
    } else if (main_led_idx == 0) {
        bounce_reverse = false;
        switchMainLed(false);
    } else {
        switchMainLed(bounce_reverse);
    }
}

int getNextIdx() {
    return (main_led_idx + 1) % LED_WAVE_SIZE;
}

int getPrevIdx() {
    return (main_led_idx + LED_WAVE_SIZE - 1) % LED_WAVE_SIZE;
}

void writeLed(uint8_t idx, uint8_t val) {
    if (idx >= 0 && idx < LED_WAVE_SIZE) {
        analogWrite(RED_LED_PINS[idx], val);
    }
}

void switchMainLed(bool reverse) {
    if (reverse) {
        main_led_idx = getPrevIdx();
    } else {
        main_led_idx = getNextIdx();
    }
}