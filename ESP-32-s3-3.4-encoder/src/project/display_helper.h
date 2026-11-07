#pragma once

#include <Arduino.h>

constexpr int ATTEMPT_LEDS[] = {15, 16, 17, 18};

void setupDisplay();
void resetLeds();