/*
 * scan.cc
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#include <Arduino.h>

extern "C" {
    #include <utility/twi.h>
}


template <typename T, size_t N>
constexpr size_t lengthof(T (&)[N]) { return N; }

class I2CScanner {
private:
    static const char* device_name(uint8_t address) {
        switch (address) {
            case 0x50: return "24C32";
            case 0x68: return "DS1307";
            default: return nullptr;
        }
    }

protected:
    virtual void device_found(uint8_t address, const char* name) const = 0;

public:
    void scan(uint8_t from_address = 1, uint8_t to_address = 127) const {
        uint8_t dummy = 0;
        for (uint8_t address = from_address; address <= to_address; address++) {
            uint8_t result = twi_writeTo(address, &dummy, 0, 1, 0);
            if (result == 0) device_found(address, device_name(address));
        }
    }
};


struct I2CPrinter : public I2CScanner {
protected:
    void device_found(uint8_t address, const char* name) const override {
        if (!Serial) return;
        Serial.print("[I2C] Address=");
        Serial.print(address, DEC);
        Serial.print(" (0x");
        Serial.print(address, HEX);
        Serial.print(')');
        if (name) {
            Serial.print(": ");
            Serial.print(name);
        }
        Serial.println();
    }
};


void setup()
{
    while (!Serial) /* nop */;
    Serial.begin(9600);

    twi_init();

    I2CPrinter printScan;
    printScan.scan();
}


void loop()
{
}

