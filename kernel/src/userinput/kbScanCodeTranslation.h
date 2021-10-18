//
// Created by Jake on 10/15/21.
//

#pragma once
#include <cstdint>

namespace QWERTYKeyboard {
    #define LeftShift 0x2a
    #define RightShift 0x36
    #define Enter 0x1c
    #define BackSpace 0x0e
    #define SpaceBar 0x39

    extern const char ASCIITable[];
    char Translate(uint8_t scancode, bool uppercase);
}
