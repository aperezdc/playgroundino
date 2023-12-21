/*
 * s7777.h
 * Copyright (C) 2017 Adrian Perez <aperez@igalia.com>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef S7777_H
#define S7777_H

#include <Arduino.h>

namespace s7777 {

#define B16(a, b) ((static_cast<uint16_t>(a) << 8) | static_cast<uint16_t>(b))
enum Bit {
    D1 = B16 (B10000000, B00000000),
    A  = B16 (B01000000, B00000000),
    F  = B16 (B00100000, B00000000),
    D2 = B16 (B00010000, B00000000),
    E  = B16 (B00001000, B00000000),
    D  = B16 (B00000100, B00000000),
    DP = B16 (B00000010, B00000000),
    C  = B16 (B00000000, B10000000),
    G  = B16 (B00000000, B01000000),
    D4 = B16 (B00000000, B00100000),
    D3 = B16 (B00000000, B00010000),
    B  = B16 (B00000000, B00001000),
    CC = B16 (B00000000, B00000100),

    MASK = (0xFFFF ^ (D1 | D2 | D3 | D4 | CC)),
};
#undef B16

enum Format {
    Decimal,
    Hex,
};

constexpr static uint16_t c(int ch)
{
    switch (ch) {
        // Digits.
        case 0: case '0': return A | B | C | D | E | F;
        case 1: case '1': return B | C;
        case 2: case '2': return A | B | G | E | D;
        case 3: case '3': return A | B | C | D | G;
        case 4: case '4': return B | C | F | G;
        case 5: case '5': return A | F | G | C | D;
        case 6: case '6': return A | C | D | E | F | G;
        case 7: case '7': return A | B | C;
        case 8: case '8': return A | B | C | D | E | F | G;
        case 9: case '9': return A | B | C | D | F | G;

        // Hex letters.
        case 0xA: case 'A': return A | B | C | E | F | G;
        case 0xB: case 'B': return C | D | E | F | G;
        case 0xC: case 'C': return A | D | E | F;
        case 0xD: case 'D': return B | C | D | E | G;
        case 0xE: case 'E': return A | D | E | F | G;
        case 0xF: case 'F': return A | E | F | G;

        // Other unambiguous letters.
        case 'H': return B | C | E | F | G;
        case 'h': return C | E | F | G;
        case 'J': return A | B | C | D | E;
        case 'L': return D | E | F;
        case 'n': return C | E | G;
        case 'o': return C | D | E | G;
        case 'P': return A | B | E | F | G;
        case 'r': return E | G;
        case 't': return D | E | F | G;
        case 'U': return B | C | D | E | F;
        case 'u': return C | D | E;
        case 'Y': return B | C | D | F | G;

        // Symbols.
        case '-': return G;
        case '=': return A | D;
        case '"': return B | F;
        case '\'':return F;
        case '.': return DP;
        case ',': return C;
        case ':': return CC;
        case ' ': return 0;
        default: return 0;
    }
}


template <uint8_t SDataPin,
          uint8_t LatchPin,
          uint8_t ClockPin>
class Controller {
private:
    uint16_t m_digits[4] = { 0, 0, 0, 0 };

public:

    inline void setup() {
        pinMode(SDataPin, OUTPUT);
        pinMode(LatchPin, OUTPUT);
        pinMode(ClockPin, OUTPUT);
        clear();
        refresh();
    }

    inline void clear() {
        m_digits[0] = 0;
        m_digits[1] = 0;
        m_digits[2] = 0;
        m_digits[3] = 0;
    }

    void display(uint16_t d1, uint16_t d2, uint16_t d3, uint16_t d4, bool colon = false) {
        m_digits[0] = D1 | (MASK & d1) | (colon ? CC : 0);
        m_digits[1] = D2 | (MASK & d2) | (colon ? CC : 0);
        m_digits[2] = D3 | (MASK & d3) | (colon ? CC : 0);
        m_digits[3] = D4 | (MASK & d4) | (colon ? CC : 0);
    }

    void display(uint8_t hi, uint8_t lo, Format fmt = Decimal) {
        switch (fmt) {
            case Decimal:
                display_decimal(hi, lo);
                break;
            case Hex:
                display_hex(hi, lo);
                break;
        }
    }

    void colon(bool enabled = true) {
        if (enabled) {
            m_digits[0] |= CC;
            m_digits[1] |= CC;
            m_digits[2] |= CC;
            m_digits[3] |= CC;
        } else {
            m_digits[0] &= ~CC;
            m_digits[1] &= ~CC;
            m_digits[2] &= ~CC;
            m_digits[3] &= ~CC;
        }
    }

    void display(const char* text) {
        m_digits[0] = D1;
        m_digits[1] = D2;
        m_digits[2] = D3;
        m_digits[3] = D4;
        for (uint8_t i = 0; i < 4 && *text; text++) {
            if (*text == '.') {
                m_digits[i == 0 ? 0 : i - 1] |= DP;
            } else {
                m_digits[i++] |= c(*text);
            }
        }
    }

    void refresh() {
        write(m_digits[0]);
        write(m_digits[1]);
        write(m_digits[2]);
        write(m_digits[3]);
    }

private:
    inline void display_decimal(uint8_t hi, uint8_t lo) {
        if (hi > 99) {
            m_digits[0] = D1 | c('E');
            m_digits[1] = D2 | c('E');
        } else {
            m_digits[0] = D1 | c(hi / 10);
            m_digits[1] = D2 | c(hi % 10);
        }
        if (lo > 99) {
            m_digits[2] = D3 | c('E');
            m_digits[3] = D4 | c('E');
        } else {
            m_digits[2] = D3 | c(lo / 10);
            m_digits[3] = D4 | c(lo % 10);
        }
    }

    inline void display_hex(uint8_t hi, uint8_t lo) {
        m_digits[0] = D1 | c(hi >> 4);
        m_digits[1] = D2 | c(hi & 0xF);
        m_digits[2] = D3 | c(lo >> 4);
        m_digits[3] = D4 | c(lo & 0xF);
    }

    inline void write(uint16_t bits) const {
        digitalWrite(LatchPin, LOW);
        shiftOut(SDataPin, ClockPin, MSBFIRST,
                 static_cast<uint8_t>(bits >> 8));
        shiftOut(SDataPin, ClockPin, MSBFIRST,
                 static_cast<uint8_t>(0xFF & bits));
        digitalWrite(LatchPin, HIGH);
    }
};

}; // namespace s7777

#endif /* !S7777_H */
