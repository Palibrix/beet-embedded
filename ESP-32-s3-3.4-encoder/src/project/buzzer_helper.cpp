#include "buzzer_helper.h"

void playNote(Note note) {
    if (note.frequency <= 0) {
        noTone(BUZZER_CHANNEL);
    } else {
        tone(BUZZER_CHANNEL, note.frequency, note.duration);
    }
    delay(note.duration);
}

void setupBuzzer() {
    ledcSetup(BUZZER_CHANNEL, NOTE_C4, PWM_RESOLUTION);
    ledcAttachPin(BUZZER_PIN, BUZZER_CHANNEL);
    ledcWrite(BUZZER_CHANNEL, 0);
}
