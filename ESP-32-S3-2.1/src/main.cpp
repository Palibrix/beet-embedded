#include <Arduino.h>

struct Config {
    static constexpr uint8_t LED_PIN = 16;
    static constexpr uint8_t BUTTON_PIN = 17;
    static constexpr uint32_t BLINK_INTERVAL_MS = 500;
    static constexpr uint32_t DEBOUNCE_TIME_MS = 50;
};

static uint32_t lastPressTime = 0;
static uint32_t lastBlinkTime = 0;
static volatile bool isButtonPressed = false;

void blink();

enum LedState {
    BLINKING,
    ON,
    OFF,
    NUM_STATES
};

class Led {
public:
    LedState mode = BLINKING;
    boolean is_on = false;

    void init() {
        pinMode(Config::LED_PIN, OUTPUT);
        set(OFF);
    }

    void set(LedState s) {
        mode = s;
        if (mode == ON) {
            digitalWrite(Config::LED_PIN, HIGH);
            is_on = true;
        } else if (mode == OFF) {
            digitalWrite(Config::LED_PIN, LOW);
            is_on = false;
        }
    }

    void toggle() {
        digitalWrite(Config::LED_PIN, is_on ? LOW : HIGH);
        is_on = !is_on;
    }
};

static Led led;

void onButtonPress() {
    isButtonPressed = true;
}

void setup() {
    led.init();
    pinMode(Config::BUTTON_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(Config::BUTTON_PIN), onButtonPress, RISING);
}

void loop() {
    uint32_t now = millis();
    if (isButtonPressed) {
        isButtonPressed = false;
        if (now - lastPressTime >= Config::DEBOUNCE_TIME_MS) {
            lastPressTime = now;
            auto new_state = static_cast<LedState>((led.mode + 1) % NUM_STATES);
            led.set(new_state);
        }
    }

    if (led.mode == BLINKING && now - lastBlinkTime >= Config::BLINK_INTERVAL_MS) {
        lastBlinkTime = now;
        led.toggle();
    }
}
