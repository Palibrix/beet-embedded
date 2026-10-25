#include <Arduino.h>

#define RES_PIN 4
#define LED_PIN 15

void setup() {
    pinMode(LED_PIN, OUTPUT);
    analogReadResolution(12);
    Serial.begin(115200);
}

void loop() {
    double rawData = analogRead(RES_PIN);
    double voltageCalibrated = analogReadMilliVolts(RES_PIN);
    double voltageCalculated = (rawData / 4095) * 3300;

    Serial.println(rawData);
    Serial.println(voltageCalculated);
    Serial.println(voltageCalibrated);

    Serial.println("Calculation error %: " + String((voltageCalibrated - voltageCalculated) / voltageCalculated * 100));

    Serial.println();
    delay(100);
}