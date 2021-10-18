//
// Created by Jake on 10/13/21.
//
#pragma once

#include <cstdint>
#include "efiMemory.h"

uint64_t GetMemorySize(EFI_MEMORY_DESCRIPTOR* mMap, uint64_t mMapSize, uint64_t mMapDescSize);
void memset(void* start, uint8_t value, uint64_t num);