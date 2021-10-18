//
// Created by avery on 10/10/21.
//

#pragma once

#include <cstddef>

struct Framebuffer {
    void* BaseAddress;
    size_t BufferSize;
    unsigned int Width;
    unsigned int Height;
    unsigned int PixelsPerScanLine;
};