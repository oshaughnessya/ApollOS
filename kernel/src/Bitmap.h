//
// Created by Jake on 10/13/21.
//

#pragma once

#include <cstddef>
#include <cstdint>

class Bitmap {
public:
    size_t Size;
    uint8_t* Buffer;
    bool operator[](uint64_t index);
    bool Set(uint64_t index, bool value);

private:
    bool Get(uint64_t index);
};


