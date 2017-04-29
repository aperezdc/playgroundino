/*
 * knight.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>


template <uint8_t PIN_SDATA,
          uint8_t PIN_LATCH,
          uint8_t PIN_CLOCK>
struct SerialByteOutput
{
    inline void setup() const {
        pinMode(PIN_SDATA, OUTPUT);
        pinMode(PIN_CLOCK, OUTPUT);
        pinMode(PIN_LATCH, OUTPUT);
        write(0);
    }

    inline void write(uint8_t bits) const {
        digitalWrite(PIN_LATCH, LOW);
        shiftOut(PIN_SDATA, PIN_CLOCK, LSBFIRST, bits);
        digitalWrite(PIN_LATCH, HIGH);
    }

    inline void operator()(uint8_t bits) const {
        write(bits);
    }
};

static SerialByteOutput<2, 3, 5> output;


void setup()
{
    output.setup();
}

void loop()
{
    uint8_t bits = 1;
    for (uint8_t i = 0; i < 7; i++) {
        output(bits);
        bits <<= 1;
        delay(50);
    }
    for (uint8_t i = 0; i < 7; i++) {
        output(bits);
        bits >>= 1;
        delay(50);
    }
}
