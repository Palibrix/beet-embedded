#include "Arduino.h"
#include <driver/ledc.h>

#define NOTE_C4  262
#define NOTE_D4  294
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523

#define BUZZER_CHANNEL LEDC_CHANNEL_0
#define BUZZER_PIN 7
#define PWM_RESOLUTION LEDC_TIMER_10_BIT

struct Note {
    uint16_t frequency;
    uint16_t duration;
};

const Note melody[] = {
    {NOTE_C4, 400}, {NOTE_C4, 400},
    {NOTE_G4, 400}, {NOTE_G4, 400},
    {NOTE_A4, 400}, {NOTE_A4, 400},
    {NOTE_G4, 800},
    {NOTE_F4, 400}, {NOTE_F4, 400},
    {NOTE_E4, 400}, {NOTE_E4, 400},
    {NOTE_D4, 400}, {NOTE_D4, 400},
    {NOTE_C4, 800},
};

void playNote(Note note) {
    if (note.frequency <= 0) {
        ledcWrite(BUZZER_CHANNEL, 0);
    } else {
        ledcWriteTone(BUZZER_CHANNEL, note.frequency);
    }

    delay(note.duration);

    ledcWrite(BUZZER_CHANNEL, 0);
    delay(50);
}

void setup() {
    Serial.begin(115200);

    ledcSetup(BUZZER_CHANNEL, NOTE_C4, PWM_RESOLUTION);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWrite(BUZZER_CHANNEL, 0);
}

void loop() {
    for (const Note note : melody) {
        playNote(note);
    }
    delay(2000);
}
