/*
 * clock.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "s7777.h"
#include <RTClib.h>

static RTC_DS1307 rtc;
static s7777::Controller<2, 4, 6> display;
static auto last_millis = millis();

void setup()
{
    display.setup();

    if (!rtc.begin() || !rtc.isrunning()) {
        while (!Serial) /* nop */;
        Serial.begin(9600);
        Serial.println("Couldn't find RTC");
        for (;;) /* nop */;
    }
}

void loop()
{
    display.refresh();
    auto current_millis = millis();
    if (current_millis - last_millis > 250) {
        last_millis = current_millis;
        auto time = rtc.now();
        display.display(time.hour(), time.minute());
        display.colon();
        display.refresh();
    }
}
