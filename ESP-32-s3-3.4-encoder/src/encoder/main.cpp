#include <Arduino.h>
#include <Bounce2.h>
#include <RotaryEncoder.h>

#define LED_PIN 15
#define ENCODER_PIN_SW 16
#define ENCODER_PIN_DT 17
#define ENCODER_PIN_CLK 18

static RotaryEncoder encoder(ENCODER_PIN_DT, ENCODER_PIN_CLK, RotaryEncoder::LatchMode::FOUR3);
static Bounce2::Button button;
static int previousPosition = 0;

void setup() {
    Serial.begin(9600);

    button.attach(ENCODER_PIN_SW, INPUT_PULLUP);
    button.interval(20);
    button.setPressedState(LOW);

    pinMode(LED_PIN, OUTPUT);

}

void loop() {
    encoder.tick();
    button.update();


    int currentPosition = encoder.getPosition();
    if (currentPosition != previousPosition) {
        previousPosition = currentPosition;
        Serial.println(currentPosition);
        digitalWrite(LED_PIN, LOW);
    }

    if (button.pressed()) {
        digitalWrite(LED_PIN, HIGH);
        encoder.setPosition(0);
        previousPosition = 0;
        Serial.println("Button pressed");
    }

}