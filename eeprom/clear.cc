/*
 * clear.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>
#include <EEPROM.h>

void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);

    // Clear all the bytes.
    for (unsigned i = 0; i < EEPROM.length(); i++)
        EEPROM.write(i, 0x00);

    // Light up the LED when done.
    digitalWrite(LED_BUILTIN, HIGH);
}

void loop()
{
    // Empty.
}
