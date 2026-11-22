#include <Arduino.h>
#include "RTClib.h"

RTC_DS1307 rtc;

void setup () {
    Serial.begin(9600);
    delay(1500);
    if (!rtc.begin()) {
        Serial.println("Failed to initialize RTC");
        while (1);
    }

    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    DateTime now = rtc.now();
    Serial.print("Date & Time: ");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(now.dayOfTheWeek());
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.println(now.second(), DEC);

}

void loop () {
}