#include <Arduino.h>
#include <Bounce2.h>

#define BAUD_RATE 115200
#define TX_PIN PB6
#define RX_PIN PB7

#define LED_PIN PC13
#define BUTTON_PIN PA0

Uart SerialVCP(RX_PIN, TX_PIN);
static Bounce2::Button button;

static bool ledState = LOW;

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(LED_PIN, OUTPUT);

    button.attach(BUTTON_PIN, INPUT_PULLUP);
    button.interval(20);
    button.setPressedState(LOW);

    SerialVCP.begin(BAUD_RATE);
}

void loop() {
    button.update();

    if (button.pressed())  SerialVCP.println("Led switch");

    if (SerialVCP.available()) {
        String line = SerialVCP.readStringUntil('\n');
        line.trim();
        if (line == "Led switch") {
            ledState = !ledState;
            digitalWrite(LED_PIN, ledState);
        }
    }

}