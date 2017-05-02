/*
 * test.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include "s7777.h"

static s7777::Controller<2, 4, 6> display;


void setup()
{
    display.setup();
    display.display("-3.14");
}

void loop()
{
    display.refresh();
}
