#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <Adafruit_BME280.h>
#include <Bounce2.h>
#include "RTClib.h"

#define BUTTON_PIN PA0

#define MASTER_SDA_PIN PB7
#define MASTER_SCL_PIN PB6

#define DISPLAY_ADDR 0x3c
#define BME280_ADDR 0x76

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

#define SEALEVELPRESSURE_HPA (1013.25)

#define TIME_UPDATE_MS 1000
#define INFO_UPDATE_MS 2000
#define BTN_HOLD_MS 800

static TwoWire I2CBus(MASTER_SDA_PIN, MASTER_SCL_PIN);
static Adafruit_SSD1306 display(DISPLAY_WIDTH, DISPLAY_HEIGHT, &I2CBus);
static Adafruit_BME280 bme;
static Bounce2::Button button;

static uint32_t lastTimeRefreshMs = 0;
static uint32_t lastInfoRefreshMs = 0;

static bool isOnHold = false;

RTC_DS1307 rtc;

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

enum TimeMode {
    TIME,
    DATE
};

static InfoMode currentInfoMode = ROOM;
static TimeMode currentTimeMode = TIME;


void clearTimeScreen() {
    display.fillRect(63, 0, display.width(), 10, SSD1306_BLACK);
}

void clearInfoScreen() {
    display.fillRect(0, 0, 61, 10, SSD1306_BLACK);
    display.fillRect(0, 12, display.width(), display.height(), SSD1306_BLACK);
}

static void switchInfoMode() {
    switch (currentInfoMode) {
        case ROOM:
            currentInfoMode = WORLD;
            break;
        case WORLD:
            currentInfoMode = ROOM;
            break;
    }
}

static void switchTimeMode() {
    switch (currentTimeMode) {
        case TIME:
            currentTimeMode = DATE;
            break;
        case DATE:
            currentTimeMode = TIME;
            break;
    }
}

static void updateTime() {
    clearTimeScreen();
    display.setTextSize(1);
    display.setCursor(65, 0);

    DateTime now = rtc.now();
    switch (currentTimeMode) {
        case TIME:
            display.print(now.hour());
            display.print(":");
            display.print(now.minute());
            display.print(":");
            display.print(now.second());
            break;
        case DATE:
            display.print(now.day());
            display.print("/");
            display.print(now.month());
            display.print("/");
            display.print(now.year());
            break;
    };
    display.display();
}

static void printLine(const Line &line) {
    display.print(line.value, line.nDecimal);
    display.println(line.prefix);
}

static void updateInfo() {
    clearInfoScreen();
    switch (currentInfoMode) {
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

    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println(displayMsg);

    display.setCursor(0, 13);
    display.setTextSize(2);

    printLine(v1);
    printLine(v2);
    display.display();
}


void setup() {
    Serial.begin(9600);
    I2CBus.begin();
    delay(1000);

    if (!bme.begin(BME280_ADDR, &I2CBus)) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }

    if (!display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR)) {
        Serial.println("SSD1306 allocation failed");
        while (1);
    }

    while (!rtc.begin(&I2CBus)) {
        Serial.println("RTC initialization failed");
    }


    button.attach(BUTTON_PIN, INPUT_PULLUP);
    button.interval(30);
    button.setPressedState(LOW);

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    display.setTextColor(SSD1306_WHITE);
    display.display();
    delay(1000);
    display.clearDisplay();

    display.drawLine(62, 0, 62, 10, SSD1306_WHITE);
    display.drawLine(0, 10, display.width(), 10, SSD1306_WHITE);
}


void loop() {
    bool infoModeChanged = false;
    bool timeModeChanged = false;
    button.update();
    if (button.pressed()) isOnHold = false;

    if (button.isPressed() && !isOnHold && button.currentDuration() >= BTN_HOLD_MS) {
        isOnHold = true;
        switchTimeMode();
        timeModeChanged = true;
    }

    if (button.released() && !isOnHold) {
        switchInfoMode();
        infoModeChanged = true;
    }

    uint32_t currentMs = millis();

    if (timeModeChanged | (currentMs - lastTimeRefreshMs >= TIME_UPDATE_MS)) {
        lastTimeRefreshMs = currentMs;
        updateTime();
    }

    if (infoModeChanged | (currentMs - lastInfoRefreshMs >= INFO_UPDATE_MS)) {
        lastInfoRefreshMs = currentMs;
        updateInfo();
    }
}
