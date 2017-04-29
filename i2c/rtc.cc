/*
 * rtc.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>
#include <RTClib.h>


static RTC_DS1307 rtc;


void setup()
{
    while (!Serial) /* nop */;
    Serial.begin(9600);

    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);

    if (!rtc.begin()) {
        Serial.println("Coultn't find RTC");
        for (;;) /* nop */;
    }


    if (!rtc.isrunning()) {
        Serial.println("RTC is not running, adjusting...");
        rtc.adjust(DateTime(2017, 4, 30, 0, 17));
    }
}

void loop()
{
    DateTime now(rtc.now());

    Serial.print(now.year(), DEC);
    Serial.print('.');
    Serial.print(now.month(), DEC);
    Serial.print('.');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

    delay(3000);
}
