//
// Created by Jake on 10/14/21.
//

#pragma once

#include <cstdint>
#include "paging.h"
#include "PageMapIndexer.h"
#include "PageFrameAllocator.h"

class PageTableManager {
public:
    PageTableManager(PageTable* PML4Address);
    PageTable* PML4;
    void MapMemory(void* virtualMemory, void* physicalMemory);
};

extern PageTableManager GlobalPageTableManager;

