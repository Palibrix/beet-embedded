#include "RotaryEncoder.h"

#include "buzzer_helper.h"
#include "encoder_helper.h"
#include "display_helper.h"

static uint8_t getNewRandomNumber() {
    return random(1, 9);
}

static uint8_t valueToGuess = 0;
static uint8_t currentAttempt = 0;


void setup() {
    Serial.begin(115200);
    setupBuzzer();
    setupEncoder();
    setupDisplay();

    randomSeed(analogRead(0));

    valueToGuess = getNewRandomNumber();
}

void loop() {
    // playMelody(SUCCESS_MELODY);

    encoder.tick();
    button.update();


    int currentPosition = encoder.getPosition();
    if (currentPosition != previousPosition) {
        if (currentPosition > 9) {
            currentPosition = 0;
            encoder.setPosition(0);
        } else if (currentPosition < 0) {
            currentPosition = 9;
            encoder.setPosition(9);
        }

        if (currentPosition == valueToGuess) {
            playMelody(PING_MELODY);
        }

        previousPosition = currentPosition;
        Serial.println(currentPosition);
    }

    if (button.pressed()) {
        if (currentPosition == valueToGuess) {
            digitalWrite(ATTEMPT_LEDS[currentAttempt], HIGH);
            if (currentAttempt == 3) {
                currentAttempt = 0;
                playMelody(SUCCESS_MELODY);
                resetLeds();
            } else currentAttempt++;

        } else {
            currentAttempt = 0;
            playMelody(FAIL_MELODY);
            resetLeds();
        }

        valueToGuess = getNewRandomNumber();

        encoder.setPosition(0);
        previousPosition = 0;
    }
}