#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BME280.h>
#include <Bounce2.h>

#define BUTTON_PIN PA0

#define MASTER_SDA_PIN PB7
#define MASTER_SCL_PIN PB6

#define DISPLAY_ADDR 0x3c
#define BME280_ADDR 0x76

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define SEALEVELPRESSURE_HPA (1013.25)

#define DELAY_TIME 1000

static TwoWire I2CBus(MASTER_SDA_PIN, MASTER_SCL_PIN);
static Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2CBus);
static Adafruit_BME280 bme;
static Bounce2::Button button;

static uint32_t lastDisplayRefreshMs = 0;

struct Line {
    float value;
    String prefix;
    uint8_t nDecimal;
};

static String displayMsg;
static Line v1;
static Line v2;

enum InfoMode {
    ROOM,
    WORLD
};

static InfoMode currentMode = ROOM;

static void switchMode() {
    switch (currentMode) {
        case ROOM:
            currentMode = WORLD;
            break;
        case WORLD:
            currentMode = ROOM;
            break;
    }
}

void setup() {
    Serial.begin(9600);
    I2CBus.begin();
    delay(1000);

    if (! bme.begin(BME280_ADDR, &I2CBus)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    if (! display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        while (1);
    }
    display.setTextColor(SSD1306_WHITE);
    display.display();
    delay(1000);
    display.clearDisplay();

    button.attach(BUTTON_PIN, INPUT_PULLUP);
    button.interval(30);
    button.setPressedState(LOW);
}


void loop() {
    bool modeChanged = false;
    button.update();
    if (button.pressed()) {
        switchMode();
        modeChanged = true;
    }

    uint32_t currentMs = millis();
    if (modeChanged | (currentMs - lastDisplayRefreshMs >= DELAY_TIME)) {
        lastDisplayRefreshMs = currentMs;

        switch (currentMode) {
            case ROOM:
                displayMsg = "ROOM INFO";

                v1.value = bme.readTemperature();
                v1.prefix = "  C";
                v1.nDecimal = 1;

                v2.value = bme.readHumidity();
                v2.prefix = "  %";
                v2.nDecimal = 1;
                break;
            case WORLD:
                displayMsg = "WORLD INFO";
                v1.value = bme.readPressure() / 100.0F;
                v1.prefix = " hPa";
                v1.nDecimal = 1;

                v2.value = bme.readAltitude(SEALEVELPRESSURE_HPA);
                v2.prefix = "   m";
                v2.nDecimal = 0;
                break;
        }
        display.clearDisplay();

        display.setCursor(0, 0);
        display.setTextSize(1);

        display.println(displayMsg);
        display.drawLine(0, 10, display.width(), 10, SSD1306_WHITE);

        display.setCursor(0, 13);
        display.setTextSize(2);

        display.print(v1.value, v1.nDecimal);
        display.println(v1.prefix);

        display.print(v2.value, v2.nDecimal);
        display.println(v2.prefix);

        display.display();
    }

}
