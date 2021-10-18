//
// Created by Jake on 10/14/21.
//

#pragma once

#include <cstdint>

class PageMapIndexer {
public:
    PageMapIndexer(uint64_t virtualAddress);
    uint64_t PDP_i;
    uint64_t PD_i;
    uint64_t PT_i;
    uint64_t P_i;
};


