/*
 * rgbfade.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>

template <typename T, size_t N>
constexpr size_t lengthof(T (&)[N]) { return N; }

enum {
    PIN_R = 3,
    PIN_G = 5,
    PIN_B = 6,
};

enum {
    L_MIN = 0,
    L_MAX = 192,
};

void setup()
{
    pinMode(PIN_R, OUTPUT);
    pinMode(PIN_G, OUTPUT);
    pinMode(PIN_B, OUTPUT);
    digitalWrite(PIN_R, LOW);
    digitalWrite(PIN_G, LOW);
    digitalWrite(PIN_B, LOW);
}

void loop()
{
    static const int pins[] = { PIN_R, PIN_G, PIN_B };

    for (uint8_t i = 0; i < lengthof(pins); i++) {
        for (int v = L_MIN; v <= L_MAX; v++) {
            analogWrite(pins[i], v);
            delay(5);
        }
    }
    for (uint8_t i = 0; i < lengthof(pins); i++) {
        for (int v = L_MAX; v > L_MIN; v--) {
            analogWrite(pins[i], v);
            delay(5);
        }
    }
}
