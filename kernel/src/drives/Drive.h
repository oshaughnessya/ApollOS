//
// Created by Jake on 10/21/21.
//

#pragma once
#include <cstdint>

class Drive {
public:
    virtual void* Read(uint64_t sector, uint32_t sectorCount) = 0;
    virtual bool Write(uint64_t sector, void* buffer) = 0;
    virtual ~Drive();
};

