#include <Arduino.h>
#include <driver/ledc.h>

#define LED_PIN 15
#define POTENTIOMETER_PIN 4

float filteredValue = 0;
float k = 0.01;

void setup() {
    Serial.begin(115200);

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_10_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = 1000,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_channel_config_t channel_config = {
        .gpio_num = LED_PIN,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = LEDC_CHANNEL_0,
        .timer_sel = LEDC_TIMER_0,
        .duty = 512,
        .hpoint = 0
    };

    pinMode(POTENTIOMETER_PIN, INPUT);
    analogReadResolution(10);
    ledc_timer_config(&timer_config);
    ledc_channel_config(&channel_config);
}

void loop() {
    float pValue = analogRead(POTENTIOMETER_PIN);
    filteredValue += (pValue - filteredValue) * k;
    auto cValue = static_cast<uint16_t>(filteredValue);
    Serial.println(cValue);

    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, cValue);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // delay(50);
}