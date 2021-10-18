//
// Created by Jake on 10/11/21.
//

#include "cstr.h"

char uintTo_strOutput[128];
const char* to_string(uint64_t value) {
    uint8_t size;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        uintTo_strOutput[size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    uintTo_strOutput[size - index] = remainder + '0';
    uintTo_strOutput[size + 1] = 0;

    return uintTo_strOutput;
}

// for some reason 'A' decides to be 10 characters below what it should be
char hexTo_StrOutput64[64 / 4 + 1];
const char* to_hstring(uint64_t value) {
    uint64_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 64 / 4 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StrOutput64[size - (i * 2 + 1)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
        tmp = ((*ptr & 0x0F));
        hexTo_StrOutput64[size - (i * 2 + 0)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
    }
    hexTo_StrOutput64[size + 1] = 0;

    return hexTo_StrOutput64;
}

char hexTo_StrOutput32[32 / 4 + 1];
const char* to_hstring(uint32_t value) {
    uint32_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 32 / 4 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StrOutput32[size - (i * 2 + 1)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
        tmp = ((*ptr & 0x0F));
        hexTo_StrOutput32[size - (i * 2 + 0)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
    }
    hexTo_StrOutput32[size + 1] = 0;

    return hexTo_StrOutput32;
}

char hexTo_StrOutput16[16 / 4 + 1];
const char* to_hstring(uint16_t value) {
    uint16_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 16 / 4 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StrOutput16[size - (i * 2 + 1)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
        tmp = ((*ptr & 0x0F));
        hexTo_StrOutput16[size - (i * 2 + 0)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
    }
    hexTo_StrOutput16[size + 1] = 0;

    return hexTo_StrOutput16;
}

char hexTo_StrOutput8[8 / 4 + 1];
const char* to_hstring(uint8_t value) {
    uint8_t* valPtr = &value;
    uint8_t* ptr;
    uint8_t tmp;
    uint8_t size = 8 / 4 - 1;
    for (uint8_t i = 0; i < size; i++) {
        ptr = ((uint8_t*)valPtr + i);
        tmp = ((*ptr & 0xF0) >> 4);
        hexTo_StrOutput8[size - (i * 2 + 1)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
        tmp = ((*ptr & 0x0F));
        hexTo_StrOutput8[size - (i * 2 + 0)] = tmp + (tmp < 10 ? '0' : ('A' - 10));
    }
    hexTo_StrOutput8[size + 1] = 0;

    return hexTo_StrOutput8;
}

char intTo_strOutput[128];
const char* to_string(int64_t value) {
    uint8_t isNegative = 0;
    if (value < 0) {
        isNegative = 1;
        value *= -1;
        intTo_strOutput[0] = '-';
    }

    uint8_t size = 0;
    uint64_t sizeTest = value;
    while (sizeTest / 10 > 0) {
        sizeTest /= 10;
        size++;
    }

    uint8_t index = 0;
    while (value / 10 > 0) {
        uint8_t remainder = value % 10;
        value /= 10;
        intTo_strOutput[isNegative + size - index] = remainder + '0';
        index++;
    }
    uint8_t remainder = value % 10;
    intTo_strOutput[isNegative + size - index] = remainder + '0';
    intTo_strOutput[isNegative + size + 1] = 0;

    return intTo_strOutput;
}
