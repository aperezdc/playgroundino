/*
 * pwmfade.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>

void setup()
{
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);
}

enum {
    DELAY = 10,
};

void loop()
{
    for (uint8_t value = 0; value < 255; value++) {
        analogWrite(3, value);
        delay(DELAY);
    }
    for (uint8_t value = 255; value > 0; value--) {
        analogWrite(3, value);
        delay(DELAY);
    }
}
