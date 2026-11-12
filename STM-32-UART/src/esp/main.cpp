#include <Arduino.h>
#include <Bounce2.h>
#include <HardwareSerial.h>

#define BAUD_RATE 115200
#define BUTTON_PIN 0
#define LED_PIN 6

#define TX_PIN 17
#define RX_PIN 18

static HardwareSerial uart1(1);
static Bounce2::Button button;

static bool ledState = LOW;

// It works, I didn't have time to make it good (nor I have Ukrainian language keycaps)

void setup() {
    Serial.begin(BAUD_RATE);
    uart1.begin(BAUD_RATE, SERIAL_8N1, RX_PIN, TX_PIN);
    button.attach(BUTTON_PIN);
    button.interval(20);
    button.setPressedState(LOW);

    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    button.update();

    if (button.pressed())  uart1.println("Led switch");

    if (uart1.available()) {
        String line = uart1.readStringUntil('\n');
        line.trim();
        if (line == "Led switch") {
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
        }
    }


    // if (button.pressed()) uart1.write(HIGH);
    // else if (button.released()) uart1.write(LOW);

}
