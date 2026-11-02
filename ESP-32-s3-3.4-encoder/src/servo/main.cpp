#include <Arduino.h>
#include <ESP32Servo.h>

#define POTENTIOMETER_PIN 4
#define SERVO_PIN 5

static float fValue = 0;
static uint16_t cValue = 0;
static float k = 0.1;

Servo servo;

void setup() {
    Serial.begin(115200);

    pinMode(POTENTIOMETER_PIN, INPUT);
    analogReadResolution(10);

    servo.attach(SERVO_PIN);
}

void loop() {
    float potValue = analogRead(POTENTIOMETER_PIN);
    fValue += (potValue - fValue) * k;
    cValue = static_cast<int>(fValue);
    Serial.println(cValue);

    int angle = map(cValue, 0, 1022, 0, 180);
    servo.write(angle);

}
