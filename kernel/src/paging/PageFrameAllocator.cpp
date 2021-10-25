//
// Created by Jake on 10/13/21.
//

#include "PageFrameAllocator.h"
#include "../memory/memory.h"

#define EFI_CONVENTIONAL_MEMORY 7

uint64_t freeMemory;
uint64_t reservedMemory;
uint64_t usedMemory;
bool Initialized = false;

PageFrameAllocator GlobalAllocator;

void PageFrameAllocator::ReadEFIMemoryMap(EFI_MEMORY_DESCRIPTOR* mMap, size_t mMapSize, size_t mMapDescSize) {
    if (Initialized) return;

    Initialized = true;

    uint64_t mMapEntries = mMapSize / mMapDescSize;

    void* largestFreeMemSeg = nullptr;
    size_t largestFreeMemSegSize = 0;

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == EFI_CONVENTIONAL_MEMORY) {
            if (desc->numPages * PAGE_SIZE > largestFreeMemSegSize) {
                largestFreeMemSeg = desc->physAddr;
                largestFreeMemSegSize = desc->numPages * PAGE_SIZE;
            }
        }
    }

    uint64_t memorySize = GetMemorySize(mMap, mMapSize, mMapDescSize);
    freeMemory = memorySize;
    uint64_t bitmapSize = memorySize / PAGE_SIZE / 8 + 1;

    // initialize bitmap
    InitBitmap(bitmapSize, largestFreeMemSeg);

    ReservePages(0, memorySize / PAGE_SIZE + 1);

    for (int i = 0; i < mMapEntries; i++) {
        EFI_MEMORY_DESCRIPTOR* desc = (EFI_MEMORY_DESCRIPTOR*)((uint64_t)mMap + (i * mMapDescSize));
        if (desc->type == EFI_CONVENTIONAL_MEMORY) {
            UnreservePages(desc->physAddr, desc->numPages);
        }
    }

    ReservePages(0, 0x100); // reserve between 0 and 0x100000
    LockPages(PageBitmap.Buffer, PageBitmap.Size / PAGE_SIZE + 1);
}

void PageFrameAllocator::InitBitmap(size_t bitmapSize, void* bufferAddress) {
    PageBitmap.Size = bitmapSize;
    PageBitmap.Buffer = (uint8_t*)bufferAddress;
    for (int i = 0; i < bitmapSize; i++) {
        *(uint8_t*)(PageBitmap.Buffer + i) = 0;
    }
}

// TODO: Optimize
uint64_t pageBitmapIndex = 0;
void* PageFrameAllocator::RequestPage() {
    for (; pageBitmapIndex < PageBitmap.Size * 8; pageBitmapIndex++) {
        if (PageBitmap[pageBitmapIndex]) continue;
        LockPage((void*)(pageBitmapIndex * PAGE_SIZE));
        return (void*)(pageBitmapIndex * PAGE_SIZE);
    }

    return nullptr; // TODO: Page frame Swap to file
}

void* PageFrameAllocator::RequestPages(size_t pageCount) {
    size_t cnt = 0;
    for (size_t i = pageBitmapIndex; i < pageBitmapIndex + pageCount; i++) {
        if (!PageBitmap[i]) {
            cnt++;
        }
    }
    if (cnt == pageCount) {
        LockPages((void*)(pageBitmapIndex * PAGE_SIZE), pageCount);
        return (void*)(pageBitmapIndex * PAGE_SIZE);
    }

    for (uint64_t i = pageBitmapIndex + pageCount; i < PageBitmap.Size * 8; i++) {
        if (!PageBitmap[i - pageCount]) {
            cnt--;
        }
        if (!PageBitmap[i]) {
            cnt++;
        }

        if (cnt == pageCount) {
            LockPages((void*)((i - pageCount) * PAGE_SIZE), pageCount);
            return (void*)((i - pageCount) * PAGE_SIZE);
        }
    }
        return nullptr;
}

void PageFrameAllocator::FreePage(void* address) {
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (!PageBitmap[index]) return;
    if (PageBitmap.Set(index, false)) {
        freeMemory += PAGE_SIZE;
        usedMemory -= PAGE_SIZE;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::FreePages(void* address, uint64_t pageCount) {
    for (int i = 0; i < pageCount; i++) {
        FreePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void PageFrameAllocator::LockPage(void* address) {
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (PageBitmap[index]) return;
    if (PageBitmap.Set(index, true)) {
        freeMemory -= PAGE_SIZE;
        usedMemory += PAGE_SIZE;
    }
}

void PageFrameAllocator::LockPages(void* address, uint64_t pageCount) {
    for (int i = 0; i < pageCount; i++) {
        LockPage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void PageFrameAllocator::UnreservePage(void* address) {
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (!PageBitmap[index]) return;
    if (PageBitmap.Set(index, false)) {
        freeMemory += PAGE_SIZE;
        reservedMemory -= PAGE_SIZE;
        if (pageBitmapIndex > index) pageBitmapIndex = index;
    }
}

void PageFrameAllocator::UnreservePages(void* address, uint64_t pageCount) {
    for (int i = 0; i < pageCount; i++) {
        UnreservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void PageFrameAllocator::ReservePage(void* address) {
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (PageBitmap[index]) return;
    if (PageBitmap.Set(index, true)) {
        freeMemory -= PAGE_SIZE;
        reservedMemory += PAGE_SIZE;
    }
}

void PageFrameAllocator::ReservePages(void* address, uint64_t pageCount) {
    for (int i = 0; i < pageCount; i++) {
        ReservePage((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

uint64_t PageFrameAllocator::GetFreeRam() {
    return freeMemory;
}

uint64_t PageFrameAllocator::GetReservedRam() {
    return reservedMemory;
}

uint64_t PageFrameAllocator::GetUsedRam() {
    return usedMemory;
}