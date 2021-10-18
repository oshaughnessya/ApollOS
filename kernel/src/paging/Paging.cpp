//
// Created by Jake on 10/15/21.
//

#include "paging.h"

void PageDirectoryEntry::SetFlag(PT_Flag flag, bool enabled) {
    uint64_t bitSelection = (uint64_t)1 << flag;
    value &= ~bitSelection;
    if (enabled) {
        value |= bitSelection;
    }
}

bool PageDirectoryEntry::GetFlag(PT_Flag flag) {
    uint64_t bitSelector = (uint64_t)1 << flag;
    return (value & bitSelector) > 0;
}

uint64_t PageDirectoryEntry::GetAddress() {
    return (value & 0x000ffffffffff000) >> 12;
}

void PageDirectoryEntry::SetAddress(uint64_t address) {
    address &= 0x000000ffffffffff;
    value &= 0xfff0000000000fff;
    value |= (address << 12);
}