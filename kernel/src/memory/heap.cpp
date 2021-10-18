//
// Created by Jake on 10/16/21.
//

#include "heap.h"
#include "../paging/PageTableManager.h"
#include "../paging/PageFrameAllocator.h"

void* heapStart;
void* heapEnd;
HeapSegHdr* LastHdr;

void InitializeHeap(void* heapAddress, size_t pageCount) {
    void* pos = heapAddress;

    for (size_t i = 0; i < pageCount; i++) {
        GlobalPageTableManager.MapMemory(pos, GlobalAllocator.RequestPage());
        pos = (void*)((size_t)pos + 0x1000);
    }

    size_t heapLength = pageCount * 0x1000;

    heapStart = heapAddress;
    heapEnd = (void*)((size_t)heapStart + heapLength);
    HeapSegHdr* startSeg = (HeapSegHdr*)heapAddress;
    startSeg->length = heapLength - sizeof(HeapSegHdr);
    startSeg->next = nullptr;
    startSeg->last = nullptr;
    startSeg->free = true;
    LastHdr = startSeg;
}

void* malloc(size_t size) {
    if (size % 0x10 > 0) {
        size -= (size % 0x10);
        size += 0x10;
    }

    if (size == 0) {
        return nullptr;
    }

    HeapSegHdr* currentSeg = (HeapSegHdr*)heapStart;
    while(true) {
        if (currentSeg->free) {
            if (currentSeg->length > size) {
                currentSeg->Split(size);
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
            }
            if (currentSeg->length == size) {
                currentSeg->free = false;
                return (void*)((uint64_t)currentSeg + sizeof(HeapSegHdr));
            }
        }
        if (currentSeg->next == nullptr) break;
        currentSeg = currentSeg->next;
    }
    ExpandHeap(size);
    return malloc(size);
}

void free(void* address) {
    HeapSegHdr* heapSegHdr = (HeapSegHdr*)((size_t)address - sizeof(HeapSegHdr));
    heapSegHdr->free = true;
    heapSegHdr->CombineForward();
    heapSegHdr->CombineBackward();
}

void ExpandHeap(size_t length) {
    if (length % 0x1000 > 0) {
        length -= length % 0x1000;
        length += 0x1000;
    }

    size_t pageCount = length / 0x1000;
    HeapSegHdr* newSegment = (HeapSegHdr*)heapEnd;

    for (size_t i = 0; i < pageCount; i++) {
        GlobalPageTableManager.MapMemory(heapEnd, GlobalAllocator.RequestPage());
        heapEnd = (void*)((size_t)heapEnd + 0x1000);
    }

    newSegment->free = true;
    newSegment->last = LastHdr;
    LastHdr->next = newSegment;
    LastHdr = newSegment;
    newSegment->next = nullptr;
    newSegment->length = length - sizeof(HeapSegHdr);
    newSegment->CombineBackward();

}

HeapSegHdr* HeapSegHdr::Split(size_t splitLength) {
    if (splitLength < 0x10) {
        return nullptr;
    }
    int64_t splitSegLength = length - splitLength - sizeof(HeapSegHdr);
    if (splitSegLength < 0x10) {
        return nullptr;
    }

    HeapSegHdr* newSplitHdr = (HeapSegHdr*)((size_t)this + splitLength + sizeof(HeapSegHdr));
    next->last = newSplitHdr;
    newSplitHdr->next = next;
    next = newSplitHdr;
    newSplitHdr->last = this;
    newSplitHdr->length = splitSegLength;
    newSplitHdr->free = free;
    length = splitLength;

    if (LastHdr == this) {
        LastHdr = newSplitHdr;
    }

    return newSplitHdr;
}

void HeapSegHdr::CombineForward() {
    if (next == nullptr || !next->free) {
        return;
    }

    if (next == LastHdr) {
        LastHdr = this;
    }

    if (next->next != nullptr) {
        next->next->last = this;
    }

    length = length + next->length + sizeof(HeapSegHdr);
    next = next->next;
}

void HeapSegHdr::CombineBackward() {
    if (last != nullptr && last->free) {
        last->CombineForward();
    }
}

