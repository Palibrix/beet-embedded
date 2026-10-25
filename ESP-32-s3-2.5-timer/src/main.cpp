#include <Arduino.h>
#define MOTOR_PIN 15
#define MOTOR_ON_DURATION_S 2
#define MOTOR_OFF_DURATION_S 10
#define DIVIDER 80
#define ALARM_VALUE 1000000

static hw_timer_t * timer = nullptr;
volatile int counter = 0;
volatile int prev_counter = 0;
volatile boolean isMotorOn = false;
volatile boolean switchMode = false;


void IRAM_ATTR onTimer() {
    counter++;
    if (isMotorOn && counter >= MOTOR_ON_DURATION_S) {
        isMotorOn = false;
        counter = 0;
    }
    else if (!isMotorOn && counter >= MOTOR_OFF_DURATION_S) {
        isMotorOn = true;
        counter = 0;
    }
}

void setup() {
    Serial.begin(9600);

    pinMode(MOTOR_PIN, OUTPUT);

    timer = timerBegin(0, DIVIDER, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, ALARM_VALUE, true);
    timerAlarmEnable(timer);
}

void loop() {
    digitalWrite(MOTOR_PIN, isMotorOn);
    if (counter != prev_counter) {
        Serial.println(counter);
        prev_counter = counter;
    }
}