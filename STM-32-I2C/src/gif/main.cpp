#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "bits.h"

#define SCREEN_W 128
#define SCREEN_H 64
#define OLED_ADDR 0x3c

#define MASTER_SDA_PIN PB7
#define MASTER_CLK_PIN PB6

static TwoWire I2CBus(MASTER_SDA_PIN, MASTER_CLK_PIN);
static Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &I2CBus);

// Video to images: https://videotoimg.com/
// Images cropping: https://batchimagecrop.com/pixels
// Images to bitmap: https://javl.github.io/image2cpp/

void setup() {
    Serial.begin(9600);
    while (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        delay (1000);
    }

    display.display();
    delay(2000);
    display.clearDisplay();
}

void loop() {
    for (const uint8_t *pic: epd_bitmap_allArray) {
        display.clearDisplay();
        display.drawBitmap(0, 0, pic, SCREEN_W, SCREEN_H, SSD1306_WHITE);
        display.display();
        delay(80);
    }


}