#include <Arduino.h>

#define LED_PIN 2
#define PHOTORESISTOR_PIN 1

float filter = 0;
float k = 0.1;
constexpr float THRESHOLD_ON = 1500.0f;
constexpr float THRESHOLD_OFF = 1700.0f;

bool lightOn = false;

void setup() {
    Serial.begin(9600);
    delay(1000);
    pinMode(LED_PIN, OUTPUT);
    pinMode(PHOTORESISTOR_PIN, INPUT);
}

void loop() {
    float lightLevel = analogRead(PHOTORESISTOR_PIN);
    filter += (lightLevel - filter) * k;
    Serial.println(filter);
    
    if (lightOn == true) {
        if (THRESHOLD_OFF < filter) {
            lightOn = false;
        }
    } else {
        if (filter < THRESHOLD_ON) {
            lightOn = true;
        }
    }
    digitalWrite(LED_PIN, lightOn);
}
